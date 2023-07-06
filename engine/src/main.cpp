#include <iostream>
#include <converter_json.h>
#include <inverted_index.h>
#include <search_server.h>

//Name SLIPE( super light integrated and power engine)

int main()
{
    engine::InvertedIndex index(engine::ConverterJSON::GetTextDocument());
    engine::SearchServer server(index);
    std::vector<std::vector<engine::RelativeIndex>> ser(server.search(engine::ConverterJSON::GetRequests()));
    engine::ConverterJSON::putAnswers(ser);
}


