#include "error.hpp"

error::error()
    : error("Unspecified error") {
    
}

error::error(const std::string& message)
    : error("Error", message) {
    
}

error::error(const std::string& title, const std::string& message)
    : error(title, message, MB_OK | MB_ICONERROR) {

}

error::error(const std::string& title, const std::string& message, const UINT message_box_type)
    : title(title), message(message), message_box_type(message_box_type) {

}

error::error(const std::wstring& message)
    : error(L"Error", message) {
    
}

error::error(const std::wstring& title, const std::wstring& message)
    : error(title, message, MB_OK | MB_ICONERROR) {

}

error::error(const std::wstring& title, const std::wstring& message, const UINT message_box_type)
    : wtitle(title), wmessage(message), message_box_type(message_box_type) {

}

const char* error::what() const noexcept {
    return message.c_str();
}

void error::display() const noexcept {
    int retval;

    if(!message.empty()) {
        retval = MessageBoxA(NULL, message.c_str(), title.c_str(), message_box_type);
    } else {
        retval = MessageBoxW(NULL, wmessage.c_str(), wtitle.c_str(), message_box_type);
    }

    if(retval == 0) {
        exit(EXIT_FAILURE);
    }
}

void error::display_uncaught_exceptions() {
    std::set_terminate([]() {
        auto eptr = std::current_exception();
        try {
            if(eptr) {
                std::rethrow_exception(eptr);
            }
        } catch(error& e) {
            e.display();
        } catch(std::exception& e) {
            error(e.what()).display();
        }

        exit(EXIT_FAILURE);
    });
}
