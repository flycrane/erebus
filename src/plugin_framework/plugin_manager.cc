#include "plugin_framework/plugin_manager.h"

#include <string>
#include <stdint.h>

#include "plugin_framework/dynamic_library.h"
#include "plugin_framework/plugin.h"
#include "path.h"
#include "directory.h"

static bool isValid(const uint8_t* objectType, const RegisterParams* params) {
        if(!objectType || !(*objectType))
                return false;
        if(!params || !params->createFunction || !params->destroyFunction)
                return false;
        return true;
}

int32_t PluginManager::registerObject(const uint8_t* objectType,
                                       const RegisterParams* params) {
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
        platformServices_.invokeService = NULL;
        platformServices_.registerObject = registerObject;
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

int32_t PluginManager::initializePlugin(InitPluginFunc initFunc) {
        PluginManager& pm=getInstance();

        ExitPluginFunc exitFunc = initFunc(&pm.platformServices_);
        if(!exitFunc)
                return -1;

        pm.exitFuncVec_.push_back(exitFunc);
        return 0;
}

int32_t PluginManager::loadAll(const std::string& pluginDirectory, InvokeServiceFunc func) {
        if(pluginDirectory.empty())
                return -1;

        platformServices_.invokeService = func;

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

        InitPluginFunc initFunc = (InitPluginFunc)(d->getSymbol("initPlugin"));
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

        ObjectParams np;
        np.objectType = (const uint8_t*)objectType.c_str();
        np.platformServices = &platformServices_;

        if(exactMatchMap_.find(objectType) != exactMatchMap_.end()) {
                RegisterParams& rp = exactMatchMap_[objectType];
                void* object = rp.createFunction(&np);
                if(object) {
                        if(rp.programmingLanguage == ProgrammingLanguage_C)
                                object = adapter.adapt(object, rp.destroyFunction);

                        return object;
                }
        }

        for(std::size_t i=0; i<wildCardVec_.size();i++) {
                RegisterParams& rp = wildCardVec_[i];
                void* object = rp.createFunction(&np);
                if(object) {
                        if(rp.programmingLanguage==ProgrammingLanguage_C)
                                object=adapter.adapt(object, rp.destroyFunction);

                        int32_t res= registerObject(np.objectType, &rp);
                        if(res<0) {
                                rp.destroyFunction(object);
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
