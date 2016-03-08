#pragma once

#include <stdexcept>
#include <string>

class PluginException : public std::runtime_error {
        public:
                explicit PluginException(const std::string& what);
};
