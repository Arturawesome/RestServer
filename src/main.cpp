#include <iostream>
#include "httplib.h"
#include <nlohmann/json.hpp>
int main(){
    httplib:: Server server;

    server.Get("/hello", [](const httplib::Request&, httplib::Response& res)
                                {
                                res.set_content("Hello world! \n", "text/plain");
                                } );
    server.listen("0.0.0.0", 8080);


    return 0;
}
