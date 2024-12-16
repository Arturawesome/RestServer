#include <iostream>
#include "httplib.h"
#include <nlohmann/json.hpp>
int main(){
    //object of server
    httplib::Server server;


    server.Get("/hello", [](const httplib::Request& req, httplib::Response& res){
        std::cout<<"Recived request: "<<req.path<<std::endl;
                                res.set_content("Hello world :) \n", "text/plain");
                            });

    server.Get("/info", [](const httplib::Request&, httplib::Response & res){
        nlohmann::json info = { {"server", "RestServer"},{"version", "0.1"}, {"author", "ArturAwesome"} };
        res.set_content(info.dump(), "application/json");
    });

    std::cout<<"Server is running on http://localhost:8080\n";
    server.listen("0.0.0.0", 8080);
    std::cout<<"Server is running on http://localhost:8080\n";


    return 0;
}
