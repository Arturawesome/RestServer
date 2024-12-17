#include <iostream>
#include <filesystem> //std for work with files in system
#include "httplib.h"
#include <nlohmann/json.hpp>


const std::string ROOT_DIR = "../data";

void get_list_function(const httplib::Request& req, httplib::Response& res);
void get_file_info(const httplib::Request& req, httplib::Response& res);
void put_file_func(const httplib::Request& req, httplib::Response& res);

int main(){
    httplib::Server server;
    server.Get("/hello", [](const httplib::Request& req, httplib::Response& res) {
        res.set_content("Hello from main4.cpp", "text/plain");
    });

    server.Get("/list", get_list_function);
    server.Get(R"(/file/(.+))", get_file_info);
    server.Put(R"(/file/(.+))", put_file_func);

    if (!std::filesystem::exists(ROOT_DIR)) {
        std::filesystem::create_directory(ROOT_DIR);
    }

    server.listen("0.0.0.0", 8080);
    return 0;
}




void put_file_func(const httplib::Request& req, httplib::Response& res){
    std::string file_name = req.matches[1].str();
    std::string file_path = std::filesystem::path(ROOT_DIR)/file_name;

    std::ofstream f(file_path, std::ios::binary);
    if (f){
        f<<req.body;
        res.set_content("File is created", "text/plain");
        return;
    }
    res.status = 500;
    res.set_content("Error in file creating", "text/plain");

}

void get_file_info(const httplib::Request& req, httplib::Response& res){
    std::string file_name = req.matches[1].str();
    auto file_path = std::filesystem::path(ROOT_DIR)/file_name;
    std::cout << "Requested file: " << file_path << std::endl;

    if(std::filesystem::exists(file_path) && std::filesystem::is_regular_file(file_path)){
        std::ifstream f(file_path, std::ios::binary);
        if(!f){
            res.status = 500;
            res.set_content("Unable to read the file", "text/plain");
            return;
        }
        std::string content((std::istreambuf_iterator<char>(f)),
                            std::istreambuf_iterator<char>());
        res.set_content(content, "text/plain");

    } else {
        res.status = 404;
        res.set_content("File does not exist", "text/plain");
    }
}

void get_list_function(const httplib::Request& req, httplib::Response& res){
    std::string answer = "Files in data directory: \n";
    if(std::filesystem::exists(ROOT_DIR) && std::filesystem::is_directory(ROOT_DIR)){
        res.set_content("Directory exists \n", "text/plain");
        for(auto f: std::filesystem::directory_iterator(ROOT_DIR)){
            answer += f.path().filename().string() + "\n";
        }
    } else {
        res.status = 500;
        res.set_content("Directory does not exist", "text/plain");
        return;
    }
    res.set_content(answer, "text/plain");
}









