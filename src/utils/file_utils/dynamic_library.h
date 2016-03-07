#pragma once

#include <string>
#include <memory>

class DynamicLibrary {
  public:
          /**
           * Loads the dynamic library at the specified path.
           * Throws if an error occurs.
           */
	static std::unique_ptr<DynamicLibrary> load(const std::string& path);

    /**
     * Constructor.
     * Should not be used.
     */
	DynamicLibrary(void* handle);
	DynamicLibrary(const DynamicLibrary&) = delete;

	~DynamicLibrary();
    
    /**
     * Returns a symbol in the loaded library.
     * Return null if the symbol does not exist.
     */
	void* getSymbol(const std::string& name);

  private:
	void* handle_;

};
