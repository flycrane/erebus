#pragma once

#include <stdint.h>

class PlatformServiceHandler {
        public:
                static int32_t invokePlatformService(const char* serviceName, void* params);

        private:
                PlatformServiceHandler();

};
