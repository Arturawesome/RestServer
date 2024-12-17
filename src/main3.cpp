#include <iostream>
#include <filesystem> //std for work with files in system
#include "httplib.h"
#include <nlohmann/json.hpp>

void get_list_function(const httplib::Request& req, httplib::Response& res);
void get_list_function2(const httplib::Request& req, httplib::Response& res);
void get_file_info(const httplib::Request& req, httplib::Response& res);

//set the data directiry
const std::string ROOT_DIR = "../data";



int main(){
    httplib::Server server;

    //path for get the list of files in data directory
    server.Get("/list", get_list_function);
    server.Get("/list2", get_list_function2);

    server.Get(R"(/file/(.+))", get_file_info);

    server.listen("0.0.0.0", 8080);
    return 0;
}










void get_file_info(const httplib::Request& req, httplib::Response& res){
    /*
    std::string content = "req.matches \n";
    for(int i = 0; i < req.matches.size(); ++i){
        content += req.matches[i]; content += "\n";
    } res.set_content(content, "text/plain"); */

    auto file_name = req.matches[1];
    auto file_path =std::filesystem::path(ROOT_DIR)/file_name;

    if(std::filesystem::exists(file_path) && std::filesystem::is_regular_file(file_path)){
        std::ifstream f(file_path, std::ios::binary);
        if(!f){
            res.status = 500;
            res.set_content("Unable to read the file", "text/plain");
            return;
        } else {
            std::string content((std::istreambuf_iterator<char>(f)),
                                std::istreambuf_iterator<char>());
            res.set_content(content, "text/plain");
        }
    } else {
        res.status = 404;
        res.set_content("File not found", "text/plain");
        return;
    }



}



void get_list_function2(const httplib::Request& req, httplib::Response& res){
    std::string answer = "Files in data directory: \n";
    if(std::filesystem::exists(ROOT_DIR) && std::filesystem::is_directory(ROOT_DIR)){
        res.set_content("Directory exists \n", "text/plain");
        for(auto f : std::filesystem::directory_iterator(ROOT_DIR)){
            answer += f.path().filename().string() + "\n";
        }

    }
    else{
        res.status = 500;
        res.set_content("Directory not found", "text/plain");
        return;
    }
        res.set_content(answer, "text/plain");
}


void get_list_function(const httplib::Request& req, httplib::Response& res){
    std::string answer = "{\"Files in data directory\":[";
    bool first_flag = true;
    //check directory
    if(std::filesystem::exists(ROOT_DIR) && std::filesystem::is_directory(ROOT_DIR)){
        std::cout<<"The directory exists:";
        for(const auto& f:std::filesystem::directory_iterator(ROOT_DIR)){
            if(!first_flag) answer += ",";
            answer += "\"" + f.path().filename().string() + "\"";
            first_flag = false;
        }
    }
    else{
        res.status = 500; // server mistake
        res.set_content("Directory not found", "text/plain");
        return;
    }
    answer += "]}";
    res.set_content(answer, "application/json");
}
