#include <inverted_index.h>
#include <search_server.h>
#include <gtest/gtest.h>

TEST(TestCaseSearchServer, TestSimple)
{
    const std::vector<std::string> docs =
    {
        "milk milk milk milk water water water",
        "milk water water",
        "milk milk milk milk milk water water water water water",
        "americano cappuccino"
    };
    const std::vector<std::string> request = {"milk water", "sugar"};
    const std::vector<std::vector<engine::RelativeIndex>> expected =
    {
        {
         {2, 1},
         {0, 0.7},
         {1, 0.3}
        },
        {

        }
    };
    engine::InvertedIndex idx(docs);

    engine::SearchServer srv(idx);
    std::vector<std::vector<engine::RelativeIndex>> result(srv.search(request));

    ASSERT_EQ(result, expected);
}

TEST(TestCaseSearchServer, TestTop5)
{
    const std::vector<std::string> docs =
    {
        "london is the capital of great britain",
        "paris is the capital of france",
        "berlin is the capital of germany",
        "rome is the capital of italy",
        "madrid is the capital of spain",
        "lisboa is the capital of portugal",
        "berlin is the capital of swetzerland",
        "moscow is the capital of russia",
        "kiev is the capital of ukraine",
        "minsk is the capital of belarus",
        "astana is the capital of kazakstan",
        "beijing is the capital of china",
        "tokyo is the capital of japan",
        "bangkok is the capital os thailand",
        "welkome to moskow the capital of russia the third rome",
        "amsterdam is the capital os netherlands",
        "helsinki is the capital of finland",
        "oslo is the capital of norway",
        "stockholm is the capital of sweden",
        "riga is the capital of latvia",
        "tallinn is the capital of estonia",
        "warsaw is the capital of poland",
    };
    const std::vector<std::string> request = {"moscow is the capital of russia"};

    const std::vector<std::vector<engine::RelativeIndex>> expected =
    {
        {
         {7, 1},
         {14, 1},
         {0, 0.666666687},
         {0, 0.666666687},
         {0, 0.666666687}
        }
    };

    engine::InvertedIndex idx;
    idx.UpdateDocumentBase(docs);

    engine::SearchServer srv(idx);

    std::vector<std::vector<engine::RelativeIndex>> result = srv.search(request);

    ASSERT_EQ(result, expected);
}

int main(int argc, char** argv)
{
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
