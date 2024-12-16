#include <iostream>
#include <filesystem>
#include "httplib.h"
#include <nlohmann/json.hpp>

void get_list_function(const httplib::Request& req, httplib::Response& res);

const std::string ROOT_DIR = "./data";

int main(){
    httplib::Server server;

    //path for get the list of files in data directory
    server.Get("/list", get_list_function);

    server.listen("0.0.0.0", 8080);
    return 0;
}


void get_list_function(const httplib::Request& req, httplib::Response& res){
    std::string answer = "Files in data directory: \n";

    //check directory
    if(std::filesystem::exists(ROOT_DIR) && std::filesystem::is_directory(ROOT_DIR)){
        std::cout<<"yes";
    }

}
