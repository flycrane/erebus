#pragma once

#include <string>

class DynamicLibrary {
  public:
	static DynamicLibrary* load(const std::string& path, std::string& errorString);

	DynamicLibrary(const DynamicLibrary&) = delete;
	~DynamicLibrary();

	void* getSymbol(const std::string& name);

  private:
	DynamicLibrary(void* handle);

	void* handle_;

};
