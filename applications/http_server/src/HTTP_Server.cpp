#include <ctime>
#include <sstream>
#include <iomanip>
#include "applications/http_server/include/HTTP_Server.h"

std::string getDate() {
    std::time_t t = std::time(nullptr);
    std::tm tm = *std::gmtime(&t);
    std::stringstream out;
    out << std::put_time(&tm, "%a, %d %b %Y %H:%M:%S %Z");
    return out.str();
}

std::string HTTP_Server::get_response_header(const ResponseCode &code) {
    std::string ok_prefix  = "HTTP/1.1 200 OK\r\n";
    std::string ok_content = "<html>You did it.</html>\r\n";

    std::string bad_request_prefix = "HTTP/1.1 400 Bad request\r\n";
    std::string bad_request_content = "<html>The request was malformed.</html>\r\n";


    std::string forbidden_prefix = "HTTP/1.1 403 Forbidden\r\n";
    std::string forbidden_content = "<html>You don't have enough privileges.</html>\r\n";

    std::string not_found_prefix = "HTTP/1.1 404 Not Found\r\n";
    std::string not_found_content = "<html>The file you requested doesn't exist.</html>\r\n";

    std::string timeout_prefix = "HTTP/1.1 408 Request Timeout\r\n";
    std::string timeout_content = "<html>Be faster next time.</html>\r\n";

    std::string too_long_prefix = "HTTP/1.1 413 Payload Too Large\r\n";
    std::string too_long_content = "<html>Are you sure you are sending a header? Too long.</html>\r\n";

    std::string invalid_http_version_prefix = "HTTP/1.1 505 HTTP Version Not Supported\r\n";
    std::string invalid_http_version_content = "<html>Only HTTP/1.1 is supported.</html>\r\n";


    std::string response_to_fill = "%s"
                                   "Date: %s\r\n"
                                   "Server: http_server/1.0.0 (%s)\r\n"
                                   "Content-Length: %s\r\n"
                                   "Content-Type: text/html\r\n"
                                   "Connection: Close\r\n"
                                   "\r\n"
                                   "%s";
    std::string prefix;
    std::string content;
    switch (code) {
        case ResponseCode::OK:
            prefix  = ok_prefix;
            content = ok_content;
            break;
        case ResponseCode::BAD_REQUEST:
            prefix  = bad_request_prefix;
            content = bad_request_content;
            break;
        case ResponseCode::FORBIDDEN:
            prefix  = forbidden_prefix;
            content = forbidden_content;
            break;
        case ResponseCode::NOT_FOUND:
            prefix  = not_found_prefix;
            content = not_found_content;
            break;
        case ResponseCode::TIMEOUT:
            prefix  = timeout_prefix;
            content = timeout_content;
            break;
        case ResponseCode::TOO_LONG:
            prefix  = too_long_prefix;
            content = too_long_content;
            break;
        case ResponseCode::INVALID_HTML_VERSION:
            prefix  = invalid_http_version_prefix;
            content = invalid_http_version_content;
            break;
    }
    std::string os = "linux";
    int len = snprintf(nullptr, 0,
                        response_to_fill.c_str(),
                        prefix.c_str(),
                        getDate().c_str(),
                        os.c_str(),
                        std::to_string(content.size()).c_str(),
                        content.c_str());
    char* res = new char[(len + 1)];
    sprintf(res,
            response_to_fill.c_str(),
            prefix.c_str(),
            getDate().c_str(),
            os.c_str(),
            std::to_string(content.size()).c_str(),
            content.c_str());
    std::string result(res);
    delete[] res;
    return result;
}
