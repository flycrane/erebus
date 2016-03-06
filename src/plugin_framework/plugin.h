#pragma once

#include <stdint.h>

extern "C" {

        /**
         * Specifies the programming langugage the plugin is written in.
         * A mix between both is not allowed.
         */
	typedef enum ProgrammingLanguage {
		ProgrammingLanguage_C,
		ProgrammingLanguage_CPP
	} ProgrammingLanguage;

    /*
     * Specifies the operating system the plugin is targeted at.
     * If a plugin is targetet at multiple operating systems, 
     * the values can be added up.
     */
    typedef enum OperatingSystem {
            OperatingSystem_Linux=1,
            OperatingSystem_Windows=2
    } OperatingSystem;            

	struct PlatformServices;

    /**
     * Holds the data that the plugin gets to create a 
     * specific object.
     */
	typedef struct ObjectParameters {
		const char*     objectType;
		const struct    PlatformServices* platformServices;
	} ObjectParameters;
    
    /**
     * Specifies the Api version of the application.
     */
	typedef struct PluginApiVersion {
		uint32_t major;
		uint32_t minor;
	} PluginApiVersion;

    /**
     * Holds the pointer to the function of the plugin that
     * creates a specific object.
     */ 
	typedef void* (*CreateObject)(ObjectParameters*);

    /**
     * Holds the pointer to the function of the plugin that
     * destroys a previous created object.
     */ 
	typedef int32_t (*DestroyObject)(void*);

    /**
     * Holds the paramters that plugin must provide in order
     * to successfully register at the plugin manager.
     */
	typedef struct RegisterParameters {
		PluginApiVersion    version;
		CreateObject        createObjectFunction;
		DestroyObject       destroyObjectFunction;
		ProgrammingLanguage programmingLanguage;
        uint8_t             operatingSystems;
	} RegisterParameters;

    /**
     * Holds the pointer to the function of the plugin manager that the plugin can call
     * in order to register an object type.
     */
	typedef int32_t (*RegisterPlugin)(const char* objectType, const RegisterParameters* params);
	
    /**
     * Holds the pointer to the function of the application that the plugin can call
     * in order to invoke service functions.
     */
    typedef int32_t (*InvokeService)(const char* serviceName, void* serviceParams);

    /**
     * Holds the pointer to the service functions of the application and plugin manager.
     */
	typedef struct PlatformServices {
		PluginApiVersion    version;
		RegisterPlugin      registerObjectFunction;
		InvokeService       invokeServiceFunction;
	} PlatformServices;

    /**
     * Holds the pointer to the function that exits the plugin.
     */
	typedef int32_t (*ExitPlugin)();

    /**
     * Holds the pointer to the function that initializes the plugin.
     */
	typedef ExitPlugin (*InitPlugin)(const PlatformServices*);

    /**
     * Function that the plugin overrides.
     * This function gets called in the plugin manager.
     */
	ExitPlugin initPlugin(const PlatformServices* params);

} // extern C
