#include <fstream>

#include <boost/scope_exit.hpp>

#include <nlohmann/json.hpp>

#include "../minhook/include/MinHook.h"

#include "../common/error.hpp"
#include "../common/winapi_error.hpp"

#include "payload.hpp"

using json = nlohmann::json;

decltype(json::parse(std::ifstream("", std::ios::in))) config;

decltype(gethostbyname)* ogethostbyname;

hostent* WINAPI hgethostbyname(const char* name) {
    return ogethostbyname(config["network"].get<std::string>().c_str());
}

DWORD WINAPI install_hook(LPVOID unused) {
    error::display_uncaught_exceptions();

    config = json::parse(std::ifstream("force-app-network.json", std::ios::in));

    if(MH_Initialize() != MH_OK) {
        throw error("Hooking system failed to initialize.");
    }

    if(MH_CreateHook(gethostbyname, hgethostbyname, reinterpret_cast<LPVOID*>(&ogethostbyname)) != MH_OK) {
        throw error("Failed to hook the function.");
    }

    if(MH_EnableHook(gethostbyname) != MH_OK) {
        throw error("Failed to enable the hook function.");
    }

    return EXIT_SUCCESS;
}

BOOL WINAPI DllMain(HINSTANCE instance, DWORD reason, LPVOID unused) {
    error::display_uncaught_exceptions();

    switch(reason) {
        case DLL_PROCESS_ATTACH: {
            if(CreateThread(NULL, 0, install_hook, NULL, 0, NULL) == NULL) {
                throw winapi_error();
            }
        } break;
        case DLL_PROCESS_DETACH: {
            if(MH_DisableHook(gethostbyname) != MH_OK) {
                throw error("Failed to unhook the function.");
            }

            if(MH_Uninitialize() != MH_OK) {
                throw error("Hooking system failed to uninitialize.");
            }
        }
    }

    return TRUE;
}
