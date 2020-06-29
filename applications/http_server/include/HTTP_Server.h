#ifndef HTTP_SERVER_HTTP_SERVER_H
#define HTTP_SERVER_HTTP_SERVER_H

#include <string>

enum class ResponseCode{
    OK                   = 200,
    BAD_REQUEST          = 400,
    FORBIDDEN            = 403,
    NOT_FOUND            = 404,
    TIMEOUT              = 408,
    TOO_LONG             = 413,
    INVALID_HTML_VERSION = 505
};

class HTTP_Server {
public:
    static std::string get_response_header(const ResponseCode& code);
};


#endif //HTTP_SERVER_HTTP_SERVER_H
