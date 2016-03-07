#include "exceptions/dynamic_link_library_exception.h"

#include <stdexcept>
#include <string>

DynamicLinkLibraryException::DynamicLinkLibraryException(const std::string& what,
                const std::string& errorString): std::runtime_error(what), errorString_(errorString) {

}

const char* DynamicLinkLibraryException::what() const noexcept {
        std::string ret=std::string(std::runtime_error::what());
        ret+=": "+errorString_;
        return ret.c_str();
}
        
