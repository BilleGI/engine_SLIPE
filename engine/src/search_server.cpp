#include <search_server.h>
#include <sstream>
#include <list>
#include <algorithm>

using AbsoluteIndex = std::map<int,int>;

bool engine::RelativeIndex::operator==(const engine::RelativeIndex &other) const
{
    return (this->rank == other.rank && this->doc_id == other.doc_id);
}

engine::SearchServer::SearchServer(const engine::InvertedIndex& idx) : index{idx}{}

void getIndividualWord(std::vector<std::vector<std::string>>& listUniqWord, const std::vector<std::string>& queries_input)
{
    for(auto& query : queries_input)
    {
        std::stringstream bufferQueries{query};
        std::string word;
        listUniqWord.emplace_back(std::vector<std::string>());
        while(bufferQueries >> word)
        {
            listUniqWord[listUniqWord.size() - 1].emplace_back(word);
            if(listUniqWord[listUniqWord.size() - 1].size() == 10) break;
        }
        if(listUniqWord.size() == 1000) break;
    }
}
void getUniqWord(std::vector<std::vector<std::string>>& listUniqWord)
{
    for(auto& uniqWord : listUniqWord)
        std::unique(uniqWord.begin(), uniqWord.end());
}

size_t countWord(const std::vector<engine::Entry>& words)
{
    if(words.empty()) return 0;

    size_t count{};

    for(auto& word : words)
        count += word.count;

    return count;
}

bool getSwap(const std::vector<engine::Entry>& left, const std::vector<engine::Entry>& right)
{
    return (countWord(left) > countWord(right));
}

// function sorted current vector words
inline void currentSorted(std::vector<std::string>& uniqWords, const std::vector<std::vector<engine::Entry>>& entry)
{
    for(size_t i{}; i < (uniqWords.size() - 1); ++i)
        for(size_t j{}; j < (uniqWords.size() - (1 + i)); ++j)
            if(getSwap(entry[j], entry[j+1]))
                std::swap(uniqWords[j], uniqWords[j+1]);
}

// this function sorted words
void sortedWords( engine::InvertedIndex& index, std::vector<std::vector<std::string>>& listUniqWord)
{
    std::vector<std::vector<std::vector<engine::Entry>>> allCollectionWordsEntry;
    for(auto& uniqWord : listUniqWord)
    {
        allCollectionWordsEntry.emplace_back(std::vector<std::vector<engine::Entry>>());
        for(auto& currentWord : uniqWord)
            allCollectionWordsEntry[allCollectionWordsEntry.size() - 1].emplace_back(index.GetWordCount(currentWord));
    }

    for(size_t i{}; i < listUniqWord.size(); ++i)
        currentSorted(listUniqWord[i], allCollectionWordsEntry[i]);
}

void initializerAnswers(AbsoluteIndex& answersOnRequest, const std::vector<engine::Entry>& wordCount)//i
{
    for(auto& word : wordCount)
    {
        if(answersOnRequest.find(word.doc_id) == answersOnRequest.end())
            answersOnRequest[word.doc_id] = word.count;
        else answersOnRequest[word.doc_id] += word.count;
    }
}

void getAbsoluteAnswer(std::vector<AbsoluteIndex>& absoluteDoc, engine::InvertedIndex& idx, const std::vector<std::vector<std::string>>& listUniqWord)
{
    for(auto& listWords : listUniqWord)
    {
        absoluteDoc.emplace_back(AbsoluteIndex());
        for(auto& word : listWords)
        {
            std::vector<engine::Entry> wordCount{idx.GetWordCount(word)};
            if(!wordCount.empty()) initializerAnswers(absoluteDoc[absoluteDoc.size()-1], wordCount);
        }
    }
}

void getMaxValue(const AbsoluteIndex& collectionAbsolute, int& maxValue)
{
    for(auto& absolute : collectionAbsolute)
        if(absolute.second > maxValue) maxValue = absolute.second;
}

void pushRelative(std::vector<engine::RelativeIndex>& collectionRelative, const AbsoluteIndex& collectionAbsolute, int& maxValue) // возможна проблема
{
    for(auto& absolute : collectionAbsolute)
        collectionRelative.emplace_back(absolute.first, ((float)absolute.second / maxValue));
}

void relativeSorted(std::vector<std::vector<engine::RelativeIndex>>& relativeDoc)
{
    for(auto& collectionRelative : relativeDoc)
    {
        if(!collectionRelative.empty())
            std::ranges::sort(collectionRelative, [](engine::RelativeIndex& left, engine::RelativeIndex& right)
                              {
                                if(left.rank == right.rank) return left.doc_id < right.doc_id;
                                return left.rank > right.rank;
            });
    }
}

void getRelativeAnswer(std::vector<std::vector<engine::RelativeIndex>>& relativeDoc, const std::vector<AbsoluteIndex>& absoluteDoc)
{
    for(auto& collectionAbsolute : absoluteDoc)
    {
        relativeDoc.emplace_back(std::vector<engine::RelativeIndex>());
        int maxValue{};
        getMaxValue(collectionAbsolute, maxValue);
        pushRelative(relativeDoc[relativeDoc.size()-1], collectionAbsolute, maxValue);
    }
    //осталась сортировка документов по id
    relativeSorted(relativeDoc);
}

std::vector<std::vector<engine::RelativeIndex>> engine::SearchServer::search(const std::vector<std::string>& queries_input)
{
    std::vector<std::vector<std::string>> listUniqWord;
    getIndividualWord(listUniqWord, queries_input);
    getUniqWord(listUniqWord);
    sortedWords(index, listUniqWord);

    std::vector<AbsoluteIndex> absoluteDoc;

    getAbsoluteAnswer(absoluteDoc, index, listUniqWord);

    listUniqWord.clear();

    std::vector<std::vector<engine::RelativeIndex>> relativeDoc;

    getRelativeAnswer(relativeDoc, absoluteDoc);

    absoluteDoc.clear();

    return relativeDoc;
}
