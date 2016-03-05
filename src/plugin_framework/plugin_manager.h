#pragma once 

#include <string>
#include <stdint.h>

#include "plugin_framework/dynamic_library.h"
#include "plugin_framework/plugin.h"

        class PluginManager {
          public:
                  static PluginManager& getInstance();
                  static uint32_t initializePlugin(InitPluginFunc initFunc);
                  
                  uint32_t loadAll(const std::string& pluginDirectory, InvokeServiceFunc func=NULL);
                  uint32_t loadByPath(const std::string& path);
                  
          private:
                  PluginManager();
                  PluginManager(const PluginManager&);
                  ~PluginManager();

                  DynamicLibrary* loadLibrary(const std::string& path, std::string& errorString);
        };

