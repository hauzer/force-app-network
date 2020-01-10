#ifndef ERROR_HPP
#define ERROR_HPP

#include <exception>
#include <string>

#include <windows.h>

class error : public std::exception {
public:
    std::string title;
    std::wstring wtitle;
    std::string message;
    std::wstring wmessage;
    UINT message_box_type;

    error();

    error(const std::string&);
    error(const std::string&, const std::string&);
    error(const std::string&, const std::string&, const UINT);

    error(const std::wstring&);
    error(const std::wstring&, const std::wstring&);
    error(const std::wstring&, const std::wstring&, const UINT);

    virtual const char* what() const noexcept override;
    virtual void display() const noexcept;

    static void display_uncaught_exceptions();
};

#endif // ERROR_HPP
