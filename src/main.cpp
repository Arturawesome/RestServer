#include <iostream>
#include "httplib.h"
#include <nlohmann/json.hpp>
int main(){
    //object of server
    httplib:: Server server;

    // path registration for get-request
    // there is the lambda function for request processing
    server.Get("/hello", [](const httplib::Request&, httplib::Response& res)
                                {
                                res.set_content("Hello world! \n", "text/plain"); //set the answer for get request
                                } );

    server.listen("0.0.0.0", 8080);


    return 0;
}
