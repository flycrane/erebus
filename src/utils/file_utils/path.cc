#include "utils/file_utils/path.h"

#include <string>

std::string Path::makeAbsolute(const std::string& path) {
        return "";
}

Path::Path(const std::string& path) {

}

std::string Path::getExtension() const {
        return "";
}

Path& Path::makeAbsolute() {
        return *this;
}

bool Path::exists() const {
        return true;
}

bool Path::isSymbolicLink() const {
        return false;
}

bool Path::isDirectory() const {
        return false;
}

std::string Path::toString() const {
    return "";
}

Path operator+(const Path& p1, const Path& p2) {
        return std::string("");
}
