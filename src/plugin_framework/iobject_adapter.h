#pragma once

#include "plugin_framework/plugin.h"

class IObjectAdapter {
        public:
                virtual ~IObjectAdapter() {}
                virtual void* adapt(void* object, DestroyPluginFunc destroyFunc) = 0;
};
