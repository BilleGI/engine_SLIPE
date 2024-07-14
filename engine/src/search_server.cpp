#include <search_server.h>
#include <list>
//#include <converter_json.h>

bool engine::RelativeIndex::operator==(const engine::RelativeIndex &other) const
{
    return (this->rank == other.rank && this->doc_id == other.doc_id);
}

engine::SearchServer::SearchServer(const engine::InvertedIndex& idx) : _index(idx){}

inline void my_swap(std::vector<std::vector<std::string>>& uniq_words_list,
                    std::vector<std::list<std::string>>& words_list)
{
    for(auto& it: words_list)
    {
        uniq_words_list.emplace_back(std::vector<std::string>());
        while(!it.empty())
        {
            uniq_words_list[uniq_words_list.size() - 1].push_back(it.front());
            it.pop_front();
        }
    }
}

void uniq_words(std::vector<std::vector<std::string>>&uniq_words_list, const std::vector<std::string>& queries_input)
{
    std::vector<std::list<std::string>> words_list;
    for(auto& it: queries_input)
    {
        std::string word;
        words_list.push_back(std::list<std::string>());
        for(auto& ch: it)
        {
            if(words_list[words_list.size()-1].size() == 10) break;
            if(ch != ' ' && ch !='\0')
            {
                word += ch;
            }
            else
            {
                words_list[words_list.size() - 1].push_back(word);
                word.clear();
            }
        }
        words_list[words_list.size()-1].unique();
        if(words_list.size() == 1000) break;
    }
    my_swap(uniq_words_list, words_list);
}

int count_word(std::vector<engine::Entry>& freq_word)
{
    if(freq_word.empty()) return 0;
    else
    {
        int count = 0;
        for(auto& it: freq_word)
        {
            count+= it.count;
        }
        return count;
    }
}

void sort_words(std::vector<std::string>& words,  engine::InvertedIndex& _index)
{
    for(int i = 0; i < words.size() - 1; ++i)
    {
        for(int j = 0; j < (words.size() - (1 + i)); ++j)
        {
            if(count_word(_index.GetWordCount(words[j])) > count_word(_index.GetWordCount(words[j+1])))
            {
                std::string word = words[j];
                words[j] = words[j+1];
                words[j+1] = word;
            }
        }
    }
}

void search_relative(std::vector<engine::RelativeIndex>& relative_words, engine::Entry& freq_word)
{
    bool search = false;
    for(auto& it: relative_words)
    {
        if(it.doc_id == freq_word.doc_id)
        {
            search = true;
            it.rank += freq_word.count;
            break;
        }
    }
    if(!search)
    {
        relative_words.push_back(engine::RelativeIndex(freq_word.doc_id, freq_word.count));
    }
}

void get_abs_relative(std::vector<engine::RelativeIndex>& relative_words, std::string& word, engine::InvertedIndex& _index)
{
    std::vector<engine::Entry> freq_word(_index.GetWordCount(word));
    if(!freq_word.empty())
    {
        if(relative_words.empty())
        {
            for(auto& it: freq_word)
                relative_words.push_back(engine::RelativeIndex(it.doc_id, it.count));
        }
        else
        {
            for(auto& it: freq_word)
            {
                search_relative(relative_words, it);
            }
        }
    }
}

void get_relative(std::vector<engine::RelativeIndex>& relative_doc)
{
    int max_relative = 0;
    for(auto& it: relative_doc)
    {
        if(max_relative < it.rank)
            max_relative = it.rank;
    }
    for(auto& it: relative_doc)
        it.rank /= max_relative;
}

void sort_doc(std::vector<engine::RelativeIndex>& relative_doc)
{
    for(int i =0; i < (relative_doc.size()-1); ++i)
    {
        for(int j = 0; j < (relative_doc.size() - (1 + i)); ++j)
        {
            if(relative_doc[j].rank < relative_doc[j+1].rank)
            {
                engine::RelativeIndex var = relative_doc[j];
                relative_doc[j] = relative_doc[j+1];
                relative_doc[j+1] = var;
            }
        }
    }
}

void relative(std::vector<engine::RelativeIndex>& relative_doc,
              std::vector<std::string>& uniq_words_list, engine::InvertedIndex& _index)
{
    sort_words(uniq_words_list, _index); // sorting by the number of words
    for(auto it: uniq_words_list)
        get_abs_relative(relative_doc, it, _index);
    get_relative(relative_doc);
    sort_doc(relative_doc);
}

std::vector<std::vector<engine::RelativeIndex>> engine::SearchServer::search(const std::vector<std::string>& queries_input)
{
    std::vector<std::vector<std::string>> uniq_words_list;
    uniq_words(uniq_words_list, queries_input);// List uniq words
    std::vector<std::vector<engine::RelativeIndex>> relative_docs;
    for(auto& it: uniq_words_list)
    {
        relative_docs.push_back(std::vector<engine::RelativeIndex>());
        relative(relative_docs[relative_docs.size()-1], it, _index);
    }
    return relative_docs;
}
