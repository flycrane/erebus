#pragma once

#include <dirent.h>
#include <string>

#include "utils/file_utils/path.h"

enum class DirectoryEntryType {
        FILE,
        DIRECTORY,
        LINK
};

class DirectoryEntry {
        public:
                DirectoryEntry(const std::string& path, DirectoryEntryType type);
                DirectoryEntry(const Path& path, DirectoryEntryType type);
                
                Path getPath() const;
                DirectoryEntryType getType() const;

        private:
                Path                path_;
                DirectoryEntryType  type_;
};

class DirectoryIterator {
        public:
                DirectoryIterator(const std::string& path);
                DirectoryIterator(const Path& path);
                ~DirectoryIterator();
                
                DirectoryIterator(const DirectoryIterator& obj) = delete;

                void reset();

                bool hasNext();
                DirectoryEntry next();

        private:
                void init();

                Path path_;
                DIR* dir_;
                bool hasNext_;
                struct dirent* nextDirEntry_;
};
