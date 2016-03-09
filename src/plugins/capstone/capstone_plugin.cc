#include "capstone_plugin.h"

#include <stdint.h>
#include <iostream>

#include <plugin_framework/plugin.h>
#include <logger.h>

extern "C" int32_t Exit() {
        //Logger::log("destroy",LogChannel::DEFAULT,LogLevel::DEBUG);
        return 0;
}

extern "C" ExitPlugin initPlugin(const PlatformServices* params) {
        LOG_INFO(LogChannel::DEFAULT,"init"); 
        params->invokeServiceFunction("testservice",NULL);
        return Exit;
}
