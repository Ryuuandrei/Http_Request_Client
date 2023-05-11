#ifndef _REQUESTS_
#define _REQUESTS_

#include <vector>
#include <string>

// computes and returns a GET request string (query_params
// and cookies can be set to NULL if not needed)
std::string compute_get_request(const char* host, const char* url, const char* query_params, std::string* cookies, std::string* auth);

// computes and returns a POST request string (cookies can be NULL if not needed)
std::string compute_post_request(const char* host, const char* url, const char* content_type, std::string body_data, std::string* cookies, std::string* auth);

std::string compute_delete_request(const char* host, const char* url, const char* query_params, std::string* cookies, std::string* auth);


#endif
