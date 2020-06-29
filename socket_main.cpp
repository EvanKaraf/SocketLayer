#include "include/Socket.h"
#include <iostream>
using namespace std;

int main() {
    Socket server(Endpoint("localhost", 8081));
    server.bind();
    server.listen();


    Socket client(Endpoint("localhost", 8081));
    client.connect();

    server.accept();

    // Start sending/receiving messages
    std::string msgToSend = "Hello this is patrick!";
    server.send(msgToSend);

    std::string msg;
    client.recv(msgToSend.size(), &msg);
    cout << msg << endl;

    msgToSend = "Lorem ipsum dolor sit amet, consectetur adipiscing elit";
    server.send(msgToSend);

    // If the sender stops responding for 3 seconds just return whatever has been received
    client.recv(9999999, &msg);
    cout << msg << endl;
    return 0;
}
