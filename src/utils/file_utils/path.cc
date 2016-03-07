#include "utils/file_utils/path.h"

#include <string>

#include "utils/file_utils/file.h"
#include "utils/file_utils/directory.h"

std::string Path::makeAbsolute(const std::string& path) {
        return "";
}

Path::Path(const std::string& path) : path_(path) {

}

std::string Path::getExtension() const {
        return "";
}

Path& Path::makeAbsolute() {
        return *this;
}

bool Path::exists() const {
      if(path_.empty())
              return false;

      return Directory::exists(path_) || File::exists(path_);
}

bool Path::isSymbolicLink() const {
        return false;
}

bool Path::isDirectory() const {
        return false;
}

std::string Path::toString() const {
    return path_;
}

Path operator+(const Path& p1, const Path& p2) {
        return std::string("");
}

Path& Path::operator+=(const Path& path) {
        return *this;
}
