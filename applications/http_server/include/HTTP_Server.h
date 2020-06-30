#ifndef HTTP_SERVER_HTTP_SERVER_H
#define HTTP_SERVER_HTTP_SERVER_H

#include <string>
#include <Socket.h>
#include <vector>
#include <poll.h>

const std::string SERVER_ADDRESS = "localhost";
const int COMMAND_PORT = 7070;
const int REQUEST_PORT = 7071;

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
    Socket request_socket;
    Socket command_socket;
    std::vector<pollfd> polls;

    int wait_for_connection();
    int is_command_connection();

public:
    static std::string create_response_header(const ResponseCode& code);

    HTTP_Server();

    void run();

};


#endif //HTTP_SERVER_HTTP_SERVER_H
