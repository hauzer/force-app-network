#include <array>
#include <fstream>

#include <windows.h>

#include <boost/scope_exit.hpp>

#include <nlohmann/json.hpp>

#include "../common/error.hpp"
#include "../common/winapi_error.hpp"

using json = nlohmann::json;

int main() {
    error::display_uncaught_exceptions();

    auto config = json::parse(std::ifstream("force-app-network.json", std::ios::in));

    PROCESS_INFORMATION target_process_info;
    {
        STARTUPINFOA startup_info{};
        startup_info.cb = sizeof(startup_info);
        if(CreateProcessA(config["app"].get<std::string>().c_str(), NULL, NULL, NULL, FALSE, 0, NULL, NULL, &startup_info, &target_process_info) == 0) {
            throw winapi_error();
        }
    }
    
    BOOST_SCOPE_EXIT_ALL(&) {
        CloseHandle(target_process_info.hThread);
        CloseHandle(target_process_info.hProcess);
    };

    std::string payload = config["payload"];
    auto payload_remote = VirtualAllocEx(target_process_info.hProcess, NULL, payload.size() + 1, MEM_COMMIT | MEM_RESERVE, PAGE_READWRITE);
    if(payload_remote == NULL) {
        throw winapi_error();
    }

    if(WriteProcessMemory(target_process_info.hProcess, payload_remote, payload.data(), payload.size() + 1, NULL) == 0) {
        throw winapi_error();
    }

    if(CreateRemoteThread(target_process_info.hProcess, NULL, 0, reinterpret_cast<LPTHREAD_START_ROUTINE>(LoadLibraryA), payload_remote, 0, NULL) == NULL) {
        throw winapi_error();
    }
}
