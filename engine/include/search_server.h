#pragma once
#include <vector>
#include <string>
#include <inverted_index.h>

namespace engine
{
    struct RelativeIndex
    {
        size_t doc_id;
        float rank;
        bool operator==(const RelativeIndex &other) const;
    };

    class SearchServer
    {
    public:
        SearchServer(const InvertedIndex& idx);
        std::vector<std::vector<RelativeIndex>> search(const std::vector<std::string>& queries_input);
    private:
        InvertedIndex _index;
    };
}
