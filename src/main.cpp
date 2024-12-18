#include <iostream>
#include <filesystem> //std for work with files in system
#include "httplib.h"
#include <nlohmann/json.hpp>


std::string ROOT_DIR;
std::string SOURCE_DIR;

void set_wrok_directory(int args, char* argv[]);
void get_list_function(const httplib::Request& req, httplib::Response& res);
void get_file_info(const httplib::Request& req, httplib::Response& res);
void put_file_func(const httplib::Request& req, httplib::Response& res);

int main(int args, char * argv[]){

    set_wrok_directory(args, argv);

    httplib::Server server;
    server.Get("/hello", [](const httplib::Request& req, httplib::Response& res) {
        res.set_content("Hello from FINUL main.cpp", "text/plain");
    });

    server.Get("/list", get_list_function);
    server.Get(R"(/file/(.+))", get_file_info);
    server.Put(R"(/file/(.+))", put_file_func);

    if (!std::filesystem::exists(SOURCE_DIR)) {
        std::filesystem::create_directory(SOURCE_DIR);
    }

    server.listen("0.0.0.0", 8080);
    return 0;
}





void set_wrok_directory(int args, char* argv[]){

    if(args>1){
        ROOT_DIR = argv[1];
    } else {
        ROOT_DIR = std::filesystem::current_path().string();
    }
    SOURCE_DIR = std::filesystem::current_path().parent_path().string() + "/data/";
    std::cout<<ROOT_DIR<<std::endl<<SOURCE_DIR<<std::endl;
}

void put_file_func(const httplib::Request& req, httplib::Response& res){
    std::string file_name = req.matches[1].str();
    std::string file_path = std::filesystem::path(SOURCE_DIR)/file_name;

    // Проверка пути на принадлежность SOURCE_DIR
    auto canonical_path = std::filesystem::canonical(file_path);
    if (canonical_path.string().find(SOURCE_DIR) != 0) {
        res.status = 403;
        res.set_content("Error 403: Access denied", "text/plain");
        return;
    }


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
    std::string file_path = std::filesystem::path(SOURCE_DIR)/file_name;
    std::cout << "Requested file: " << file_path << std::endl;

    try{
        // Проверка пути на принадлежность SOURCE_DIR
        auto canonical_path = std::filesystem::canonical(file_path);
        if (canonical_path.string().find(SOURCE_DIR) != 0) {
            res.status = 403;
            res.set_content("Error 403: Access denied", "text/plain");
            return;
        }

        if (!std::filesystem::exists(file_path)) {
            res.status = 404; // Not Found
            res.set_content("Error 404: File does not exist", "text/plain");
            return;
        }
        if (!std::filesystem::is_regular_file(file_path)) {
            // json for directory
            if (std::filesystem::is_directory(file_path)) {
                std::string json = "{ \"files\": [";
                for (const auto& entry : std::filesystem::directory_iterator(file_path)) {
                    json += "\"" + entry.path().filename().string() + "\",";
                }
                if (json.back() == ',') json.pop_back();
                json += "] }";
                res.status = 200;
                res.set_content(json, "application/json");
                return;
            } else {
                res.status = 403; // Forbidden
                res.set_content("Error 403: Access denied", "text/plain");
                return;
            }
        }

        //read file
        std::ifstream f(file_path, std::ios::binary);
        if(!f){
            res.status = 500;
            res.set_content("Unable to read the file", "text/plain");
            return;
        }
        std::string content((std::istreambuf_iterator<char>(f)),
                            std::istreambuf_iterator<char>());
        res.set_content(content, "text/plain");

    } catch (const std::exception& e) {
        res.status = 500; // Internal Server Error
        res.set_content(std::string("Error 500: ") + e.what(), "text/plain");
    }
}

void get_list_function(const httplib::Request& req, httplib::Response& res){
    std::string answer = "Files in data directory: \n";
    if(std::filesystem::exists(SOURCE_DIR) && std::filesystem::is_directory(SOURCE_DIR)){
        res.set_content("Directory exists \n", "text/plain");
        for(auto f: std::filesystem::directory_iterator(SOURCE_DIR)){
            answer += f.path().filename().string() + "\n";
        }
    } else {
        res.status = 500;
        res.set_content("Directory does not exist", "text/plain");
        return;
    }
    res.set_content(answer, "text/plain");
}









