#include "converter_json.h"
#include "my_exception.h"
#include "search_server.h"

#include <nlohmann/json.hpp>

#include <iostream>
#include <fstream>
#include <filesystem>

enum FILENAME
{
    ANSWERS = 1,
    CONFIG = 2,
    REQUESTS = 4,
};

std::string path_in_file(const FILENAME& file)
{
    std::string path = std::filesystem::current_path().parent_path().parent_path().string() + "/engine/JSON_FILES/";
    if(FILENAME::ANSWERS == file) path += "answers.json";
    else if(FILENAME::CONFIG == file) path += "config.json";
    else if(FILENAME::REQUESTS == file) path += "requests.json";
    return path;
}

inline std::ifstream open_file(const FILENAME&& file)
{
    std::ifstream config_file(path_in_file(file));
    if(!config_file.is_open())
        throw engine::FileException("config file is missing");
    return config_file;
}

nlohmann::json get_config()
{
    std::ifstream file(open_file(FILENAME::CONFIG));
    nlohmann::json  data_config = nlohmann::json::parse(file);
    file.close();
    if(data_config.find("config") == data_config.end())
        throw engine::FileException("config file is empty");
    return data_config;
}

inline void check_data(nlohmann::json& data_config)
{
    try
    {
        data_config = get_config();
    }
    catch(const engine::FileException& error)
    {
        std::cerr << "Error in file configuration: " << error.what() << std::endl;
        exit(1);
    }
}

std::string name_file(const std::string& path)
{
    std::string name;
    for(int i = path.size() - 1; i >= 0; --i)
    {
        if(path[i] == '/')
        {
            for(int j = (i+1); j < path.size(); ++j)
            {
                name += path[j];
            }
            i = -1;
        }
    }
    return name;
}

std::string docString(std::string&& path)
{
    path = std::filesystem::current_path().parent_path().parent_path().string() + "/engine" + path;
    std::ifstream resource_file(path);

    if(!resource_file.is_open())
    {
        path = "The is no file on the specigied path: " + path;
        throw path;
    }

    std::string information;
    while(resource_file.good())
    {
        std::string str;
        getline(resource_file, str);
        information += str;
    }
    if(!resource_file.eof())
    {
        if(resource_file.fail())
        {
            path = "Input terminated by data mismatch: " + name_file(path);
            throw path;
        }
        else
        {
            path = "End of file not reached: " + name_file(path);
            throw path;
        }
    }

    resource_file.close();
    return information;
}

std::vector<std::string> engine::ConverterJSON::GetTextDocument()
{
    nlohmann::json data_config;
    check_data(data_config);
    std::cout << "" << data_config["config"]["name"] << std::endl;
    std::vector<std::string> TextDocument;
    for(auto& it: data_config["files"])
    {
        try
        {
            TextDocument.push_back(docString(it.get<std::string>()));
        }
        catch (const std::string& path)
        {
            std::cerr << path << std::endl;
        }
        catch(const std::exception&)
        {
            std::cerr <<"Other error" << std::endl;
        }
    }

    return TextDocument;
}

int engine::ConverterJSON::GetResponsesLimit()
{
    nlohmann::json data_config;
    check_data(data_config);
    if(data_config["config"].find("max_responses") == data_config.end())
        return 5;
    return data_config["config"]["max_responses"];
}

std::vector<std::string> engine::ConverterJSON::GetRequests()
{
    nlohmann::json data_requests;
    std::vector<std::string> requests;
    try
    {
        data_requests = nlohmann::json::parse(open_file(FILENAME::REQUESTS));
    }
    catch(const engine::FileException& exception)
    {
        std::cerr << "Error: " << exception.what() << std::endl;
            return std::vector<std::string>();
    }
    if(data_requests.find("requests") != data_requests.end())
    {
            for(auto& it: data_requests["requests"])
            {
            requests.push_back(it.get<std::string>());
            }
    }
    return requests;
}

inline std::string number_request(int& num)
{
    if(num < 10)
            return ("00"+std::to_string(num));
    else if(num < 100)
            return ("0" + std::to_string(num));
    return std::to_string(num);
}

void ready_answers(nlohmann::json& data_answers, const std::vector<std::vector<engine::RelativeIndex>>& answers)
{
    for(int i = 0; i < answers.size() ; ++i)
    {
        std::string request = "request" + number_request(i);
        if(answers[i].empty())
        data_answers["answers"][request]["result"] = false;
        else
        {
            data_answers["answers"][request]["result"] = true;
            if(answers[i].size() == 1)
            {
                data_answers["answers"][request]["docid"] = answers[i][0].doc_id;
                data_answers["answers"][request]["rank"] = answers[i][0].rank;
            }
            else
            {
                for(auto& j : answers[i])
                {
                    data_answers["answers"][request]["relevance"].push_back({
                        {"docid", j.doc_id},
                        {"rank", j.rank}
                    });
                }
            }
        }
    }
}

inline std::ofstream file()
{
    std::string path = path_in_file(FILENAME::ANSWERS);
    std::ofstream file_answers(path);
    if(!file_answers.is_open())
        throw engine::FileException("the " + name_file(path) + " file didn`t open");
    return file_answers;
}

void engine::ConverterJSON::putAnswers(const std::vector<std::vector<engine::RelativeIndex>>& answers)
{
    nlohmann::json data_answers;
    ready_answers(data_answers, answers);
    try
    {
        std::ofstream file_answers(file());
        file_answers << data_answers;
        file_answers.close();
    }
    catch(const engine::FileException& except)
    {
        std::cerr << "Error: " << except.what() << std::endl;
    }
}
