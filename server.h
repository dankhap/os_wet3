//
// Created by daniel on 11/06/2020.
//

#ifndef OS_WET3_SERVER_H
#define OS_WET3_SERVER_H


#include <string>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include "packets.h"





class Server {

public:
    explicit Server(int port_num);
    void run() ;
    int sock;
    int msg_size;
    struct sockaddr_in server_addr;
    struct sockaddr_in client_aadr;
    uint16_t client_addr_len;
    WriteRequest write_request;
};


#endif //OS_WET3_SERVER_H

