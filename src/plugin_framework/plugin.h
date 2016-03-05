#pragma once

#include <stdint.h>

extern "C" {
        typedef enum ProgrammingLanguage {
               ProgrammingLanguage_C,
               ProgrammingLanguage_CPP
        } ProgrammingLanguage;
          
        struct PlatformServices;

        typedef struct ObjectParams {
                const uint8_t* objectType;
                const struct Platformservices* platformServices;
        } ObjectParams;

        typedef struct PluginApiVersion {
               uint32_t major;
               uint32_t minor;
        } PluginApiVersion;

        typedef void* (*CreatePluginFunc)(ObjectParams*);
        typedef int32_t (*DestroyPluginFunc)(void*);

        typedef struct RegisterParams {
                PluginApiVersion version;
                CreatePluginFunc createFunction;
                DestroyPluginFunc destroyFunction;
                ProgrammingLanguage programmingLanguage;
        } RegisterParams;

        typedef uint32_t (*RegisterPluginFunc)(const uint8_t* nodeType, const RegisterParams* params);
        typedef uint32_t (*InvokeServiceFunc)(const uint8_t* serviceName, void* serviceParams);

        typedef struct PlatformServices {
                PluginApiVersion version;
                RegisterPluginFunc registerObject;
                InvokeServiceFunc invokeService;
        } PlatformServices;

        typedef uint32_t (*ExitPluginFunc)();

        typedef ExitPluginFunc (*InitPluginFunc)(const PlatformServices*);

        ExitPluginFunc initPlugin(const PlatformServices* params);

} // extern C
