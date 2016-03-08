#include "plugin_framework/plugin_manager.h"
#include "core/platform_service_handler.h"

int main(int argc, char** argv) {
        auto& pluginManager = PluginManager::getInstance();
        
        pluginManager.setPlatformService(&PlatformServiceHandler::invokePlatformService);
        pluginManager.loadAll("plugins/");

	    return 0;
}
