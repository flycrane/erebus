#include "exceptions/plugin_exception.h"

#include <stdexcept>
#include <string>

PluginException::PluginException(const std::string& what): std::runtime_error(what) {

}
