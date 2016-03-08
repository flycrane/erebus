#include "capstone_plugin.h"

#include <stdint.h>
#include <iostream>

#include <plugin_framework/plugin.h>
#include <logger.h>

extern "C" int32_t Exit() {
        Logger::log("destroy",LoggerChannel::DEFAULT,LogLevel::DEBUG);
        return 0;
}

extern "C" ExitPlugin initPlugin(const PlatformServices* params) {
        Logger::log("init",LoggerChannel::DEFAULT,LogLevel::DEBUG);
        std::cout<<params->invokeServiceFunction("testservice",NULL)<<std::endl;
        return Exit;
}
