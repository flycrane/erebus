#include "plugin_framework/plugin_manager.h"

#include <boost/filesystem/path.hpp>
#include <boost/filesystem/operations.hpp>
#include <boost/range/iterator_range.hpp>

#include <string>
#include <stdint.h>

#include "utils/file_utils/dynamic_library.h"
#include "plugin_framework/plugin.h"

#include "exceptions/dynamic_link_library_exception.h"
#include "exceptions/plugin_exception.h"

static bool isValid(const char* objectType, const RegisterParameters* params) {
        if(!objectType || !(*objectType))
                return false;
        if(!params || !params->createObjectFunction || !params->destroyObjectFunction)
                return false;
        return true;
}

void PluginManager::setPlatformService(int32_t (*platformServiceFunc)(const char* serviceName, void* params)) {
        platformServices_.invokeServiceFunction = platformServiceFunc;
}


int32_t PluginManager::registerObject(const char* objectType,
                                       const RegisterParameters* params) {
        if(!isValid(objectType,params))
                return -1;

        PluginManager& pm = getInstance();

        PluginApiVersion v = pm.platformServices_.version;
        if(v.major != params->version.major)
                return -1;

        std::string key((const char*)objectType);

        if(pm.exactMatchMap_.find(key) != pm.exactMatchMap_.end()) {
                return -1;
        }

        pm.exactMatchMap_[key] = *params;
        return 0;
}

DynamicLibrary* PluginManager::loadLibrary(const std::string& path) {
         auto d = DynamicLibrary::load(path);

        if(!d)
                return NULL;
        
        //dynamicLibraryMap_[Path::makeAbsolute(path)] = std::shared_ptr<DynamicLibrary>(d);
        
        return d.get();
}

PluginManager& PluginManager::getInstance() {
        static PluginManager instance;

        return instance;
}

PluginManager::PluginManager() : initializePlugin_(false) {
        platformServices_.version.major=1;
        platformServices_.version.minor=0;
        platformServices_.invokeServiceFunction = NULL;
        platformServices_.registerObjectFunction = registerObject;
}

PluginManager::~PluginManager() {
        shutdown();
}

int32_t PluginManager::shutdown() {
        int32_t result=0;
        for(ExitFuncVec::iterator func=exitFuncVec_.begin(); func != exitFuncVec_.end(); ++func ) {
                try {
                        result = (*func)();
                }
                catch(...) {
                        result = -1;
                }
        }

        dynamicLibraryMap_.clear();
        exactMatchMap_.clear();
        exitFuncVec_.clear();

        return result;
}

int32_t PluginManager::initializePlugin(InitPlugin initFunc) {
        PluginManager& pm=getInstance();

        ExitPlugin exitFunc = initFunc(&pm.platformServices_);
        if(!exitFunc)
                return -1;

        pm.exitFuncVec_.push_back(exitFunc);
        return 0;
}

void PluginManager::loadAll(const std::string& pluginDirectory) {
        if(pluginDirectory.empty())
                throw std::invalid_argument("Empty plugin directory name");

        boost::filesystem::path dirPath(pluginDirectory);
        if(!boost::filesystem::exists(dirPath) || !boost::filesystem::is_directory(dirPath))
                throw std::invalid_argument("Plugin directory does not exist");
        
        for(auto& entry : boost::make_iterator_range(boost::filesystem::directory_iterator(dirPath))) {
                if(boost::filesystem::is_directory(entry)) 
                        continue;

                if(entry.path().extension() != ".so")
                        continue;
                
                auto absolutePath = boost::filesystem::canonical(entry);
                try {
                        loadByPath(absolutePath.string());
                } catch(DynamicLinkLibraryException& de) {

                } catch(PluginException& pe) {

                }
        }
}

void PluginManager::loadByPath(const std::string& pluginPath) {
        boost::filesystem::path plugin(pluginPath);

        if(!boost::filesystem::exists(plugin) || !boost::filesystem::is_regular_file(plugin))
                throw std::invalid_argument("Plugin does not exist");

        plugin = boost::filesystem::canonical(plugin);
        
        if(dynamicLibraryMap_.find(plugin.string()) != dynamicLibraryMap_.end())
                return;

        DynamicLibrary* d = loadLibrary(plugin.string());

        InitPlugin initFunc = (InitPlugin)(d->getSymbol("initPlugin"));
        if(!initFunc)
                throw DynamicLinkLibraryException("Is not a valid plugin");

        int32_t res=initializePlugin(initFunc);
        if(res<0)
                throw PluginException("Failed to initialize plugin");
}

void* PluginManager::createObject(const std::string& objectType, IObjectAdapter& adapter) {
        if(objectType == std::string("*"))
                        return NULL;

        ObjectParameters np;
        np.objectType = (const char*)objectType.c_str();
        np.platformServices = &platformServices_;

        if(exactMatchMap_.find(objectType) != exactMatchMap_.end()) {
                RegisterParameters& rp = exactMatchMap_[objectType];
                void* object = rp.createObjectFunction(&np);
                if(object) {
                        if(rp.programmingLanguage == ProgrammingLanguage_C)
                                object = adapter.adapt(object, rp.destroyObjectFunction);

                        return object;
                }
        }

        return NULL;
}

const PluginManager::RegistrationMap& PluginManager::getRegistrationMap() {
        return exactMatchMap_;
}

PlatformServices& PluginManager::getPlatformServices() {
        return platformServices_;
}
