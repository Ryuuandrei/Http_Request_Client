#include <iostream>
#include <unistd.h>     /* read, write, close */
#include <string.h>     /* memcpy, memset */
#include <sys/socket.h> /* socket, connect */
#include <netinet/in.h> /* struct sockaddr_in, struct sockaddr */
#include <netdb.h>      /* struct hostent, gethostbyname */
#include <arpa/inet.h>
#include <string>
#include <vector>
#include "helpers.h"
#include "requests.h"

std::string compute_get_request(const char* host, const char* url, const char* query_params, std::string* cookies, std::string* auth)
{
    std::string message;

    // Step 1: write the method name, URL, request params (if any) and protocol type
    char* _line = new char[LINELEN];

    if (query_params != nullptr)
        sprintf(_line, "GET %s?%s HTTP/1.1", url, query_params);
    else
        sprintf(_line, "GET %s HTTP/1.1", url);

    compute_message(message, _line);

    // Step 2: add the host
    sprintf(_line, "HOST: %s", host);
    compute_message(message, _line);

    // Step 3 (optional): add headers and/or cookies, according to the protocol format
    if (auth) {
        sprintf(_line, "Authorization: Bearer %s", auth->c_str());
        compute_message(message, _line);
    }

    if (cookies) {
        message += "Cookie: ";
        sprintf(_line, "%s", cookies->c_str());
        compute_message(message, _line);
    }
    // Step 4: add final new line
    compute_message(message, "");
    delete[] _line;
    return message;
}

std::string compute_post_request(const char* host, const char* url, const char* content_type, std::string body_data, std::string* cookies, std::string* auth)
{
    std::string message;

    // Step 1: write the method name, URL and protocol type
    char* _line = new char[LINELEN];
    sprintf(_line, "POST %s HTTP/1.1", url);
    compute_message(message, _line);
    
    // Step 2: add the host
    sprintf(_line, "HOST: %s", host);
    compute_message(message, _line);
    /* Step 3: add necessary headers (Content-Type and Content-Length are mandatory)
            in order to write Content-Length you must first compute the message size
    */
    sprintf(_line, "Content-Type: %s", content_type);
    compute_message(message, _line);

    sprintf(_line, "Content-Length: %ld", body_data.size());
    compute_message(message, _line);

     if (auth) {
        sprintf(_line, "Authorization: Bearer %s", auth->c_str());
        compute_message(message, _line);
    }
    // Step 4 (optional): add cookies
    if (cookies) {
        message += "Cookie: ";
        sprintf(_line, "%s", cookies->c_str());
        compute_message(message, _line);
    }
    // Step 5: add new line at end of header
    compute_message(message, "");

    // Step 6: add the actual payload data
    // memset(_line, 0, LINELEN);

    message += body_data;

    delete[] _line;
    return message;
}

std::string compute_delete_request(const char* host, const char* url, const char* query_params, std::string* cookies, std::string* auth) {

    std::string message;

    // Step 1: write the method name, URL, request params (if any) and protocol type
    char* _line = new char[LINELEN];

    if (query_params != nullptr)
        sprintf(_line, "DELETE %s?%s HTTP/1.1", url, query_params);
    else
        sprintf(_line, "DELETE %s HTTP/1.1", url);

    compute_message(message, _line);

    // Step 2: add the host
    sprintf(_line, "HOST: %s", host);
    compute_message(message, _line);

    // Step 3 (optional): add headers and/or cookies, according to the protocol format
    if (auth) {
        sprintf(_line, "Authorization: Bearer %s", auth->c_str());
        compute_message(message, _line);
    }

    if (cookies) {
        sprintf(_line, "Cookie: %s", cookies->c_str());
        compute_message(message, _line);
    }
    // Step 4: add final new line
    compute_message(message, "");
    delete[] _line;
    return message;

}
