//
// Created by daniel on 11/06/2020.
//

#ifndef OS_WET3_SERVER_H
#define OS_WET3_SERVER_H

#include <map>
#include <string>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include "packets.h"
#include "state.h"
#include "handler.h"


class Server {

public:
    static void printACK(packet::Ack pack);
    explicit Server(int port_num);
    void run() ;
    static int print_err(STATUS status);

    ~Server();
private:
    std::map<Opcode, Handler*> handlers;
    State s;
    int sock;
    struct sockaddr_in server_addr{};
    struct sockaddr_in client_aadr{};
    bool server_alive;


};


#endif //OS_WET3_SERVER_H

