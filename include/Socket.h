#ifndef HTTP_SERVER_SOCKET_H
#define HTTP_SERVER_SOCKET_H

#include <string>
#include <sys/socket.h>
#include <arpa/inet.h>

typedef int port_t;
const int MAX_CLIENTS = 10;

class Endpoint{
    std::string address;
public:
    const std::string& getAddress() const;

private:
    port_t port;
public:
    port_t getPort() const;

public:
    Endpoint(const std::string &address, port_t port);
};

class Connection {
    int sock;
public:
    Connection(int sock);
    Connection(Connection& conn) = delete;
    Connection(Connection&& conn);

    void shutdown();

    int getSock() const;
    bool isValid() const;

    bool recv(int msg_len, std::string* msg) const;
    bool send(const std::string& msg) const;
};


class Socket {
    Endpoint endpoint;
    int sock;
    bool is_bound;
    bool is_connected;
public:
    Socket(const Endpoint &endpoint);

    virtual ~Socket();

    bool bind();
    bool listen();
    Connection connect();
    Connection accept();

    int getSock() const;
};



#endif //HTTP_SERVER_SOCKET_H
