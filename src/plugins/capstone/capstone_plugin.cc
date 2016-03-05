#include "plugins/capstone/capstone_plugin.h"

#include <stdint.h>

#include "plugin_framework/plugin.h"

extern "C" uint32_t Exit() {
        return 0;
}

extern "C" ExitPluginFunc initPlugin(const PlatformServices* params) {
        return Exit;
}
