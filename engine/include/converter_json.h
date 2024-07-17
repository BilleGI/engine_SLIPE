#pragma once
#include <vector>
#include <string>

namespace engine
{
    struct RelativeIndex;

    class ConverterJSON
    {
    public:
        ConverterJSON() = default;
        static std::vector<std::string> GetTextDocument();
        static std::vector<std::string> GetRequests();
        static void putAnswers(const std::vector<std::vector<RelativeIndex>>&);
    };
}
