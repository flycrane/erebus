#include "plugin_framework/plugin_manager.h"
#include "core/platform_service_handler.h"
#include "logger.h"

void initLogger() {
        auto& logger=Logger::getInstance();

        logger.enableConsoleLogging();
}

int main(int argc, char** argv) {
        initLogger();

        auto& pluginManager = PluginManager::getInstance();
        
        pluginManager.setPlatformService(&PlatformServiceHandler::invokePlatformService);
        pluginManager.loadAll("plugins/");
        pluginManager.shutdown();

	    return 0;
}
