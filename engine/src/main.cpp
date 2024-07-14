#include <iostream>
#include <converter_json.h>
#include <inverted_index.h>
#include <search_server.h>

//Name SLIPE( super light integrated and power engine)

int main()
{
    engine::InvertedIndex index(engine::ConverterJSON::GetTextDocument());
    engine::SearchServer server(index);
    engine::ConverterJSON::putAnswers(server.search(engine::ConverterJSON::GetRequests()));
}


