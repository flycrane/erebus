#include "core/platform_service_handler.h"

#include <stdint.h>

#include "logger.h"

int32_t PlatformServiceHandler::invokePlatformService(const char* serviceName, void* params) {
        std::string name(serviceName);
        //Logger::log(name,LogChannel::DEFAULT,LogLevel::DEBUG);
        return 1;
}

PlatformServiceHandler::PlatformServiceHandler() {

}
