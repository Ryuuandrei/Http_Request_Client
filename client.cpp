#include <unistd.h>     /* read, write, close */
#include <string.h>     /* memcpy, memset */
#include <sys/socket.h> /* socket, connect */
#include <netinet/in.h> /* struct sockaddr_in, struct sockaddr */
#include <netdb.h>      /* struct hostent, gethostbyname */
#include <arpa/inet.h>
#include <string>
#include <iostream>
#include <algorithm>
#include "helpers.h"
#include "requests.h"
#include "nlohmann/json.hpp"

// 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 - github token

using json = nlohmann::json;

int main(int argc, char *argv[])
{   

    std::string message, response, command;
    std::string set_cookie;
    std::string jwt;
    std::vector<json> books;

    for(;;) {

        int sockfd, id;
        std::string HOST = "34.254.242.81";
        sockfd = open_connection(HOST.c_str(), 8080, AF_INET, SOCK_STREAM, 0);
        if (sockfd < 0) {
            std::cout << "open_connection\n";
            exit(-1);
        }

        std::cin >> command;

        if (command.compare("register") == 0) {

            std::string username, password;

            std::cout << "Username: ";
            std::cin >> username;
            std::cout << "Password: ";
            std::cin >> password;

            json data = {
                {"username", username},
                {"password", password}
            };
            message = compute_post_request(HOST.c_str(), "/api/v1/tema/auth/register", "application/json", data.dump(), nullptr, nullptr);

            send_to_server(sockfd, message.c_str());

            response = receive_from_server(sockfd);

            std::stringstream check(response);
            std::string intermediate;
            getline(check, intermediate, ' ');
            getline(check, intermediate, ' ');

            if (intermediate.compare("201") == 0) {
                std::cout << intermediate << " - User registered succesfully!\n";
            }

            const char* r_string = strstr(response.c_str(), "{"); 
            if (r_string != nullptr) {
                json r_data = json::parse(r_string);
                std::string err = r_data["error"];
                err.erase(std::remove(err.begin(), err.end(), '\"'), err.end());
                std::cout << err << "\n";
            }
        }

        else if (command.compare("login") == 0) {

            std::string username, password;

            std::cout << "Username: ";
            std::cin >> username;
            std::cout << "Password: ";
            std::cin >> password;

            json data = {
                {"username", username},
                {"password", password}
            };
            message = compute_post_request(HOST.c_str(), "/api/v1/tema/auth/login", "application/json", data.dump(), nullptr, nullptr);

            send_to_server(sockfd, message.c_str());

            response = receive_from_server(sockfd);

            std::stringstream check(response);
            std::string intermediate;
            getline(check, intermediate, ' ');
            getline(check, intermediate, ' ');

            if (intermediate.compare("200") == 0) {
                std::cout << intermediate << " - Logged in as " << username << ". Welcome!\n";
                char* l = (char*)strstr(response.c_str(), "Set-Cookie");
                strtok(l, " ");
                set_cookie = strtok(nullptr, " ");
                set_cookie[set_cookie.size() - 1] = '\0';
            }

            const char* r_string = strstr(response.c_str(), "{"); 
            if (r_string != nullptr) {
                json r_data = json::parse(r_string);
                std::string err = r_data["error"];
                err.erase(std::remove(err.begin(), err.end(), '\"'), err.end());
                std::cout << err << "\n";
            }
        }

        else if (command.compare("enter_library") == 0) {

            message = compute_get_request(HOST.c_str(), "/api/v1/tema/library/access", nullptr, &set_cookie, nullptr);

            send_to_server(sockfd, message.c_str());
            
            response = receive_from_server(sockfd);

            std::stringstream check(response);
            std::string intermediate;
            getline(check, intermediate, ' ');
            getline(check, intermediate, ' ');

            if (intermediate.compare("200") == 0) {
                std::cout << intermediate << " - You entered the library!\n";
            }

            const char* r_string = strstr(response.c_str(), "{"); 
            if (r_string != nullptr) {
                json r_data = json::parse(r_string);
                if (r_data["error"] != nullptr) {
                    std::string err = r_data["error"];
                    err.erase(std::remove(err.begin(), err.end(), '\"'), err.end());
                    std::cout << err << "\n";
                } else {
                    jwt = r_data["token"];
                    jwt.erase(std::remove(jwt.begin(), jwt.end(), '\"'), jwt.end());
                }
            }
        }

        else if (command.compare("get_books") == 0) {

            message = compute_get_request(HOST.c_str(), "/api/v1/tema/library/books", nullptr, nullptr, &jwt);

            send_to_server(sockfd, message.c_str());
            
            response = receive_from_server(sockfd);

            std::stringstream check(response);
            std::string intermediate;
            getline(check, intermediate, ' ');
            getline(check, intermediate, ' ');

            if (intermediate.compare("200") == 0) {
                std::cout << intermediate << " - Here are your books!\n";
            }

            const char* r_string = strstr(response.c_str(), "[");
            if (r_string != nullptr) {
                books = json::parse(r_string);
                std::cout << std::setw(4) << books << "\n";
                books.clear();
            } else {
                const char* r_string = strstr(response.c_str(), "{");
                json r_data = json::parse(r_string);
                std::string err = r_data["error"];
                err.erase(std::remove(err.begin(), err.end(), '\"'), err.end());
                std::cout << err << "\n";
            }

        }

        else if (command.compare("get_book") == 0) {

            std::cout << "id= ";
            std::cin >> id;

            std::string PATH ("/api/v1/tema/library/books/" + std::to_string(id));

            message = compute_get_request(HOST.c_str(), PATH.c_str(), nullptr, nullptr, &jwt);

            send_to_server(sockfd, message.c_str());

            response = receive_from_server(sockfd);

            std::stringstream check(response);
            std::string intermediate;
            getline(check, intermediate, ' ');
            getline(check, intermediate, ' ');

            if (intermediate.compare("200") == 0) {
                std::cout << intermediate << " - The book you were looking for!\n";
            }

            const char* r_string = strstr(response.c_str(), "{");
            if (r_string != nullptr) {
                json r_data = json::parse(r_string);
                if (r_data["error"] != nullptr) {
                    std::string err = r_data["error"];
                    err.erase(std::remove(err.begin(), err.end(), '\"'), err.end());
                    std::cout << err << "\n";
                } else {
                    std::cout << std::setw(4) <<  r_data << "\n";
                }
            }

        }

        else if (command.compare("add_book") == 0) {

            std::string title, author, genre, publisher;
            int page_count;
            std::cout <<"title= ";
            std::getline(std::cin, title);
            std::getline(std::cin, title);
            std::cout << "author= ";
            std::getline(std::cin, author);
            std::cout << "genre= ";
            std::getline(std::cin, genre);
            std::cout << "publisher= ";
            std::getline(std::cin, publisher);
            std::cout << "page_count= ";
            std::cin >> page_count;

            json data = {
                {"title", title},
                {"author", author},
                {"genre", genre},
                {"page_count", page_count},
                {"publisher", publisher}
            };


            message = compute_post_request(HOST.c_str(), "/api/v1/tema/library/books", "application/json",  data.dump(), nullptr, &jwt);

            send_to_server(sockfd, message.c_str());

            response = receive_from_server(sockfd);

            std::stringstream check(response);
            std::string intermediate;
            getline(check, intermediate, ' ');
            getline(check, intermediate, ' ');

            if (intermediate.compare("200") == 0) {
                std::cout << intermediate << " - Your book was added succesfully!\n";
            }

            const char* r_string = strstr(response.c_str(), "{"); 
            if (r_string != nullptr) {
                json r_data = json::parse(r_string);
                std::string err = r_data["error"];
                err.erase(std::remove(err.begin(), err.end(), '\"'), err.end());
                std::cout << err << "\n";
            }

        }

        else if (command.compare("delete_book") == 0) {

            std::cout << "id= ";
            std::cin >> id;

            std::string PATH ("/api/v1/tema/library/books/" + std::to_string(id));
            message = compute_delete_request(HOST.c_str(), PATH.c_str(), nullptr, nullptr, &jwt);

            send_to_server(sockfd, message.c_str());

            response = receive_from_server(sockfd);

            std::stringstream check(response);
            std::string intermediate;
            getline(check, intermediate, ' ');
            getline(check, intermediate, ' ');

            if (intermediate.compare("200") == 0) {
                std::cout << intermediate << " - Book with id " << id << " was succesfully deleted!\n";
            }

            const char* r_string = strstr(response.c_str(), "{"); 
            if (r_string != nullptr) {
                json r_data = json::parse(r_string);
                std::string err = r_data["error"];
                err.erase(std::remove(err.begin(), err.end(), '\"'), err.end());
                std::cout << err << "\n";
            }

        }

        else if (command.compare("logout") == 0) {
            
            message = compute_get_request(HOST.c_str(), "/api/v1/tema/auth/logout", nullptr, &set_cookie, nullptr);

            send_to_server(sockfd, message.c_str());

            response = receive_from_server(sockfd);

            std::stringstream check(response);
            std::string intermediate;
            getline(check, intermediate, ' ');
            getline(check, intermediate, ' ');

            if (intermediate.compare("200") == 0) {
                std::cout << intermediate << " - Good bye!\n";
                set_cookie = "";
                jwt = "";
            }

            const char* r_string = strstr(response.c_str(), "{"); 
            if (r_string != nullptr) {
                json r_data = json::parse(r_string);
                std::string err = r_data["error"];
                err.erase(std::remove(err.begin(), err.end(), '\"'), err.end());
                std::cout << err << "\n";
            }

        }

        else if (command.compare("exit") == 0) {
            close(sockfd);
            break;
        }

        close(sockfd);


    }


    return 0;
}
