# SocketLayer

A header-only higher level API abstracting Unix's socket API

## Example
```cpp
    using namespace SocketLayer;
    Socket server(Endpoint("localhost", 8080));
    server.bind();
    server.listen();


    Socket client(Endpoint("localhost", 8080));
    Connection client_con = client.connect();

    Connection server_con = server.accept()
    std::string msgToSend = "This is a message";
    server_con.send(msgToSend);

    std::string msg;
    client_con.recv(msgToSend.size(), &msg);
```

## Usage

1. Include `Socket.h` into your project
2. That's it!

## Applications

[A simple HTTP server using this API](https://github.com/EvanKaraf/SocketLayer/tree/master/applications/http_server)