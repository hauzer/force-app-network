#include "winapi_error.hpp"

winapi_error::winapi_error() {
    title = "WinAPI Error";
    
    LPSTR raw_message = NULL;
    if(FormatMessageA(
        FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_IGNORE_INSERTS,
        NULL,
        GetLastError(),
        MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
        raw_message,
        0,
        NULL
    ) == 0) {
        message = "Error formatting a GetLastError() code via FormatMessage().";
    }

    message = raw_message;
    LocalFree(raw_message);
}
