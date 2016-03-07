#include "utils/file_utils/directory.h"

#include <string>
#include <memory>

#include "utils/file_utils/path.h"
#include "utils/file_utils/directory_iterator.h"

std::unique_ptr<DirectoryIterator> Directory::iterator(const std::string& path) {
        return std::make_unique<DirectoryIterator>(path);
}

bool Directory::exists(const std::string& path) {
        return true;
}
