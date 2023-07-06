#include "my_exception.h"
#include <exception>
#include <string>


engine::FileException::FileException(const std::string& message) : message(message){}

const char* engine::FileException::what() const noexcept
{
    return message.c_str();
}
