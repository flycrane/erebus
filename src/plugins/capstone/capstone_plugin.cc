#include "plugins/capstone/capstone_plugin.h"

#include <stdint.h>

#include "plugin_framework/plugin.h"

extern "C" int32_t Exit() {
        return 0;
}

extern "C" ExitPlugin initPlugin(const PlatformServices* params) {
        return Exit;
}
