#include "plugin_framework/plugin_manager.h"

#include <string>
#include <stdint.h>

#include "plugin_framework/dynamic_library.h"
#include "plugin_framework/plugin.h"

DynamicLibrary* PluginManager::loadLibrary(const std::string& path, std::string& errorString) {
        DynamicLibrary* d = DynamicLibrary::load(path,errorString);

        if(!d)
                return NULL;

        return d;
}

PluginManager& PluginManager::getInstance() {
        static PluginManager instance;

        return instance;
}

PluginManager::PluginManager() {

}

PluginManager::~PluginManager() {

}

uint32_t PluginManager::initializePlugin(InitPluginFunc initFunc) {
        //PluginManager& pm=getInstance();

        return 0;
}

uint32_t PluginManager::loadAll(const std::string& pluginDirectory, InvokeServiceFunc func) {
        if(pluginDirectory.empty())
                return -1;
        return 0;
}

uint32_t PluginManager::loadByPath(const std::string& path) {
        return 0;
}
