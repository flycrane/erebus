#pragma once

#include <string>

#include "path.h"

namespace Directory {


struct Entry {
	enum Type {
		FILE,
		DIRECTORY,
		LINK
	};

	Type type;
	std::string path;
};

class Iterator {
  public:
	Iterator(const Path& path);
	Iterator(const std::string& path);
	~Iterator();

	Entry* next(Entry& e);
};
}
