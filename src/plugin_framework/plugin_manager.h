#pragma once

#include <string>
#include <stdint.h>
#include <vector>
#include <map>
#include <memory>

#include "utils/file_utils/dynamic_library.h"
#include "plugin_framework/plugin.h"
#include "plugin_framework/iobject_adapter.h"

class PluginManager {
	using DynamicLibraryMap = std::map<std::string, std::unique_ptr<DynamicLibrary>>;
	using ExitFuncVec = std::vector<ExitPlugin>;
	using RegistrationVec = std::vector<RegisterParameters>;

  public:
	using RegistrationMap = std::map<std::string, RegisterParameters>;
	
	PluginManager(const PluginManager&) = delete;
    ~PluginManager();

	static PluginManager& getInstance();
	static void initializePlugin(InitPlugin initFunc);
	static int32_t registerObject(const char* objectType,
	                              const RegisterParameters* params);

	void loadAll(const std::string& pluginDirector);
	void loadByPath(const std::string& pluginPath);

    void* createObject(const std::string& objectType, IObjectAdapter& adapter);
	const RegistrationMap& getRegistrationMap();
	PlatformServices& getPlatformServices();

	void shutdown();

    void setPlatformService(int32_t (*platformServiceFunc)(const char* serviceName, void* params));

  private:
	PluginManager();

    DynamicLibrary* loadLibrary(const std::string& path);

	PlatformServices platformServices_;
	DynamicLibraryMap dynamicLibraryMap_;
	ExitFuncVec exitFuncVec_;

	RegistrationMap exactMatchMap_;
};

