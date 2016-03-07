#include "utils/file_utils/dynamic_library.h"

#include <dlfcn.h>
#include <sstream>
#include <iostream>
#include <string>
#include <memory>

#include "exceptions/dynamic_link_library_exception.h"

DynamicLibrary::DynamicLibrary(void* handle) : handle_(handle) {

}

DynamicLibrary::~DynamicLibrary() {
        if(handle_) {
                dlclose(handle_);
        }
}

std::unique_ptr<DynamicLibrary> DynamicLibrary::load(const std::string& path) {
        if(path.empty()) {
                throw DynamicLinkLibraryException("Empty path");
        }

        void* handle=NULL;

        handle=dlopen(path.c_str(), RTLD_NOW);
        if(!handle) {
                std::string dlErrorString;
                const char* reterrstr=dlerror();
                if(reterrstr)
                        dlErrorString=reterrstr;
                std::string errorString= "Failded to load '"+path+"'";
                throw DynamicLinkLibraryException(errorString,dlErrorString);
        }
        return std::make_unique<DynamicLibrary>(handle);
}

void* DynamicLibrary::getSymbol(const std::string& symbol) {
     if(!handle_)
             return nullptr;

     return dlsym(handle_,symbol.c_str());
}

