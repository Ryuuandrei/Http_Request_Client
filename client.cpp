#include <unistd.h>     /* read, write, close */
#include <string.h>     /* memcpy, memset */
#include <sys/socket.h> /* socket, connect */
#include <netinet/in.h> /* struct sockaddr_in, struct sockaddr */
#include <netdb.h>      /* struct hostent, gethostbyname */
#include <arpa/inet.h>
#include <string>
#include <iostream>
#include "helpers.h"
#include "requests.h"
#include <nlohmann/json.hpp>

// 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 - github token

int main(int argc, char *argv[])
{
    std::string message, response, command;
    int sockfd, id;

    for(;;) {

        std::cin >> command;

        if (command.compare("register")) {

            std::string username, password;

            std::cout << "Username:";
            std::cin >> username;
            std::cout << "Password:";
            std::cin >> password;
        }

        // if ()
        break;

        
        
        
    }



    //
    // Ex 1.1: GET dummy from main server
    
    // Ex 1.2: POST dummy and print response from main server
    // Ex 2: Login into main server
    // Ex 3: GET weather key from main server
    // Ex 4: GET weather data from OpenWeather API
    // Ex 5: POST weather data for verification to main server
    // Ex 6: Logout from main server

    // BONUS: make the main server return "Already logged in!"

    return 0;
}
