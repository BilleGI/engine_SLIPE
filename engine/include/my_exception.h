#pragma once

#include <exception>
#include <string>


namespace engine
{
    class FileException: std::exception
    {
    public:
        FileException(const std::string& message);
        const char* what() const noexcept override;
    private:
        std::string message;
    };
}
