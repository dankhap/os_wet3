//
// Created by daniel on 11/06/2020.
//

#include "server.h"
#include <iostream>
#include <cstring>

using std::cout;
using std::endl;

void Server::run() {

    const int WAIT_FOR_PACKET_TIMEOUT = 3;
    const int NUMBER_OF_FAILURES = 7;
    do
    {
        // establish connection with client and get WRQ packet
        client_addr_len = sizeof(client_aadr);

        int write_req_len = recvfrom(sock, &write_request, MAX_PACK_SIZE, 0, (struct sockaddr *) &client_aadr,
                            (socklen_t*)(&client_addr_len));
        cout<<"got write request"<<endl;

        char name[MAX_PACK_SIZE];
        char protocol [MAX_PACK_SIZE];

        cout<<ntohs(write_request.opcode)<<endl;
        write_request.data[MAX_PACK_SIZE-3]='\0';
        strcpy(name, write_request.data);
        strcpy(protocol, (write_request.data + strlen(name) + 1));
        cout<<name<<" "<<protocol<<" "<<endl;
        exit(1);
        do
        {
            do
            {
                // TODO: Wait WAIT_FOR_PACKET_TIMEOUT to see if something appears
                // for us at the socket (we are waiting for DATA)
                if (1)// TODO: if there was something at the socket and
                    // we are here not because of a timeout
                {
                    // TODO: Read the DATA packet from the socket (at
                    // least we hope this is a DATA packet)
                }
                if (1) // TODO: Time out expired while waiting for data
                //to appear at the socket
                {
                    //TODO: Send another ACK for the last packet
                    //timeoutExpiredCount++;
                }
                if (1)//timeoutExpiredCount>= NUMBER_OF_FAILURES)
                {
                    // FATAL ERROR BAIL OUT
                }
            }while (0) ;// TODO: Continue while some socket was ready
              //but recvfrom somehow failed to read the data
            if (0) // TODO: We got something else but DATA
            {
                // FATAL ERROR BAIL OUT
            }
            if (0) // TODO: The incoming block number is not what we have
             //expected, i.e. this is a DATA pkt but the block number
             //in DATA was wrong (not last ACK’s block number + 1)
            {
                 //FATAL ERROR BAIL OUT
            }
        }while (1);
        //timeoutExpiredCount = 0;
        //lastWriteSize = fwrite(...); // write next bulk of data
// TODO: send ACK packet to the client
   }while (0); // Have blocks left to be read from client (not end of transmission)

}

Server::Server(int port_num) {

    // initilize socket
    if((sock = socket(PF_INET,SOCK_DGRAM,0))<0){
        cout<<errno;
        exit(1);
    }
    // initilize server struct
    memset(&server_addr,0, sizeof(server_addr));

    server_addr.sin_family = AF_INET;

    server_addr.sin_addr.s_addr = htonl(INADDR_ANY);

    server_addr.sin_port = htons(port_num);

    if(bind(sock,(struct sockaddr*) &server_addr, sizeof(server_addr)) < 0 ){
        cout<<"something wrong in bind";
        exit(1);
    }


    memset(&write_request,0, sizeof(write_request));
    /*listen(sock,5);

    socklen_t client_addr_length = sizeof(client_aadr);

    int new_socket = accept(sock, (struct sockaddr *)&client_aadr, &client_addr_length);

    if(new_socket<0){
        cout<<"error in new socket";
    }*/

}
