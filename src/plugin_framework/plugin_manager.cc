#include "plugin_framework/plugin_manager.h"

#include <string>
#include <stdint.h>

#include "utils/file_utils/dynamic_library.h"
#include "plugin_framework/plugin.h"
#include "utils/file_utils/path.h"
#include "utils/file_utils/directory.h"

static bool isValid(const char* objectType, const RegisterParameters* params) {
        if(!objectType || !(*objectType))
                return false;
        if(!params || !params->createObjectFunction || !params->destroyObjectFunction)
                return false;
        return true;
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
        if(key == std::string("*")) {
                pm.wildCardVec_.push_back(*params);
                return 0;
        }

        if(pm.exactMatchMap_.find(key) != pm.exactMatchMap_.end()) {
                return -1;
        }

        pm.exactMatchMap_[key] = *params;
        return 0;
}

DynamicLibrary* PluginManager::loadLibrary(const std::string& path, std::string& errorString) {
        DynamicLibrary* d = DynamicLibrary::load(path,errorString);

        if(!d)
                return NULL;
        
        dynamicLibraryMap_[Path::makeAbsolute(path)] = std::shared_ptr<DynamicLibrary>(d);
        
        return d;
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
        wildCardVec_.clear();
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

int32_t PluginManager::loadAll(const std::string& pluginDirectory, InvokeService func) {
        if(pluginDirectory.empty())
                return -1;

        platformServices_.invokeServiceFunction = func;

        Path dir_path(pluginDirectory);
        if(!dir_path.exists() || !dir_path.isDirectory())
                return -1;

        Directory::Entry e;
        Directory::Iterator di(dir_path);
        while(di.next(e)) {
                Path full_path(dir_path+Path(e.path));

                if(full_path.isDirectory())
                        continue;

                std::string ext = full_path.getExtension();
                if(ext != ".so")
                        continue;
        }
        return 0;
}

uint32_t PluginManager::loadByPath(const std::string& pluginPath) {
        Path path(pluginPath);

        if(path.isSymbolicLink()) {
                return -1;
        }

        if(dynamicLibraryMap_.find(path.toString()) != dynamicLibraryMap_.end())
                return -1;

        path.makeAbsolute();

        std::string errorString;
        DynamicLibrary* d = loadLibrary(path.toString(), errorString);
        if(!d)
                return -1;

        InitPlugin initFunc = (InitPlugin)(d->getSymbol("initPlugin"));
        if(!initFunc)
                return -1;

        int32_t res=initializePlugin(initFunc);
        if(res<0)
                return res;

        return 0;
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

        for(std::size_t i=0; i<wildCardVec_.size();i++) {
                RegisterParameters& rp = wildCardVec_[i];
                void* object = rp.createObjectFunction(&np);
                if(object) {
                        if(rp.programmingLanguage==ProgrammingLanguage_C)
                                object=adapter.adapt(object, rp.destroyObjectFunction);

                        int32_t res= registerObject(np.objectType, &rp);
                        if(res<0) {
                                rp.destroyObjectFunction(object);
                                return NULL;
                        }
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
