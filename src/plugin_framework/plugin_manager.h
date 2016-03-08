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
	using DynamicLibraryMap = std::map<std::string, std::shared_ptr<DynamicLibrary>>;
	using ExitFuncVec = std::vector<ExitPlugin>;
	using RegistrationVec = std::vector<RegisterParameters>;

  public:
	using RegistrationMap = std::map<std::string, RegisterParameters>;

	static PluginManager& getInstance();
	static int32_t initializePlugin(InitPlugin initFunc);
	static int32_t registerObject(const char* objectType,
	                              const RegisterParameters* params);

	void loadAll(const std::string& pluginDirector);
	void loadByPath(const std::string& pluginPath);

	void* createObject(const std::string& objectType, IObjectAdapter& adapter);
	const RegistrationMap& getRegistrationMap();
	PlatformServices& getPlatformServices();

	int32_t shutdown();

    void setPlatformService(int32_t (*platformServiceFunc)(const char* serviceName, void* params));

  private:
	PluginManager();
	PluginManager(const PluginManager&);
	~PluginManager();

	DynamicLibrary* loadLibrary(const std::string& path);

	bool initializePlugin_;
	PlatformServices platformServices_;
	DynamicLibraryMap dynamicLibraryMap_;
	ExitFuncVec exitFuncVec_;

	RegistrationMap exactMatchMap_;
};

