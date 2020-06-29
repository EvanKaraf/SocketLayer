#include "include/Socket.h"
#include <iostream>
#include <vector>
using namespace std;

int main() {
    Socket inbound(Endpoint("localhost", 8084));
    inbound.bind();
    inbound.listen();


    Socket client(Endpoint("localhost", 8084));
    Socket client1(Endpoint("localhost", 8084));
    Connection conn_clie = client.connect();
    Connection conn_clie2 = client1.connect();

    vector<Connection> conn_serv;
    conn_serv.push_back(inbound.accept());
    conn_serv.push_back(inbound.accept());



    // Start sending/receiving messages
    std::string msgToSend = "Hello this is patrick!";
    conn_serv[0].send(msgToSend);
    conn_serv[1].send("Patrick2");


    std::string msg;
    conn_clie.recv(5855278, &msg);
    cout << msg << endl;



    conn_clie.send("client message1");
    conn_clie2.send("client message2");

    conn_serv[0].recv(9999999, &msg);
    cout << msg << endl;
    conn_serv[1].recv(9999999, &msg);
    cout << msg << endl;

    conn_clie2.recv(msgToSend.size(), &msg);
    cout << msg << endl;

    return 0;
}

