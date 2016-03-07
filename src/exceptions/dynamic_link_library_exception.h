#pragma once

#include <stdexcept>
#include <string>

class DynamicLinkLibraryException : public std::runtime_error {
        public:
                explicit DynamicLinkLibraryException(const std::string& what, const std::string& errorString);
                
                virtual const char* what() const noexcept override;
        private:
                std::string errorString_;
};

