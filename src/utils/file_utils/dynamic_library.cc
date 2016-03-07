#include "utils/file_utils/dynamic_library.h"

#include <dlfcn.h>
#include <sstream>
#include <iostream>
#include <string>

DynamicLibrary::DynamicLibrary(void* handle) : handle_(handle) {

}

DynamicLibrary::~DynamicLibrary() {
        if(handle_) {
                dlclose(handle_);
        }
}

DynamicLibrary* DynamicLibrary::load(const std::string& path, std::string& errorString) {
        if(path.empty()) {
                errorString="Empty path";
                return NULL;
        }

        void* handle=NULL;

        handle=::dlopen(path.c_str(), RTLD_NOW);
        if(!handle) {
                std::string dlErrorString;
                const char* reterrstr=::dlerror();
                if(reterrstr)
                        dlErrorString=reterrstr;
                errorString =+ "Failded to load '"+path+"'";
                if(dlErrorString.size()) {
                        errorString += ": " + dlErrorString;
                }
                return NULL;
        }
        return new DynamicLibrary(handle);
}

void* DynamicLibrary::getSymbol(const std::string& symbol) {
     if(!handle_)
             return NULL;

     return dlsym(handle_,symbol.c_str());
}

