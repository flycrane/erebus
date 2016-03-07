#include "utils/file_utils/directory_iterator.h"

#include <string>
#include <dirent.h>

#include "utils/file_utils/path.h"

DirectoryEntry::DirectoryEntry(const std::string& path, DirectoryEntryType type):
    path_(path), type_(type) {

}

DirectoryEntry::DirectoryEntry(const Path& path, DirectoryEntryType type):
        path_(path), type_(type) {
}

DirectoryIterator::~DirectoryIterator() {
        if(dir_) {
                closedir(dir_);
        }
}

Path DirectoryEntry::getPath() const  {
        return path_;
}

DirectoryEntryType DirectoryEntry::getType() const {
        return type_;
}

DirectoryIterator::DirectoryIterator(const std::string& path):path_(path) {
        init();
}

DirectoryIterator::DirectoryIterator(const Path& path): path_(path) {
        init();
}

void DirectoryIterator::reset() {
        closedir(dir_);
        init();
}

bool DirectoryIterator::hasNext() {
        return hasNext_;
}

DirectoryEntry DirectoryIterator::next() {
        if(!hasNext()) {

        }
        Path entrypath=path_+std::string(nextDirEntry_->d_name);
        
        DirectoryEntryType type;
        switch(nextDirEntry_->d_type) {
                case DT_DIR:
                        type=DirectoryEntryType::DIRECTORY;
                        break;
                case DT_LNK:
                        type=DirectoryEntryType::LINK;
                        break;
                case DT_REG:
                        type=DirectoryEntryType::FILE;
                        break;
                default:
                        ;
                        //throw error
        }

        do {
                nextDirEntry_=readdir(dir_);
                if(!nextDirEntry_) {
                        hasNext_=false;
                        break;
                }

                if(nextDirEntry_->d_type!=DT_REG &&
                   nextDirEntry_->d_type!=DT_LNK &&
                   nextDirEntry_->d_type!=DT_DIR) {
                        nextDirEntry_=NULL;
                }
        } while(!nextDirEntry_);

        return DirectoryEntry(entrypath,type);
}

void DirectoryIterator::init() {
        hasNext_=true;
        nextDirEntry_=NULL;
        
        path_.makeAbsolute();
        dir_=opendir(path_.toString().c_str());
        if(!dir_) {
                hasNext_=false;
                return;
        }

        nextDirEntry_=readdir(dir_);
        if(!nextDirEntry_) {
                hasNext_=false;
                return;
        }
}
