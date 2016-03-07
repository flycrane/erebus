#pragma once

#include <string>
#include <memory>

#include "utils/file_utils/path.h"

class DirectoryIterator;

class Directory {
        public:
                static std::unique_ptr<DirectoryIterator> iterator(const std::string& path);
                static std::unique_ptr<DirectoryIterator> iterator(const Path& path);
};
