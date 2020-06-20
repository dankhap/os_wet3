//
// Created by daniel on 11/06/2020.
//

#include "server.h"
#include "data_handler.h"
#include "wrq_handler.h"
#include <iostream>
#include <cstring>


using std::cout;
using std::endl;

void Server::run() {

    const int WAIT_FOR_PACKET_TIMEOUT = 3;
    const int NUMBER_OF_FAILURES = 7;
    bool session_in_progress = false;
    struct timeval tv{};
    tv.tv_sec = 0;
    tv.tv_usec = WAIT_FOR_PACKET_TIMEOUT*100000;
    server_alive = true;
    int timeoutExpiredCount = 0;
    do
    {
        // establish connection with client and get WRQ packet
        socklen_t addr_len = sizeof(client_aadr);
        packet::Basic packet = {0};
        int write_req_len = recvfrom(sock, &packet, MAX_PACK_SIZE, 0,
                (struct sockaddr *) &client_aadr,
                &addr_len);
        if(write_req_len <= 0)
            continue;
        cout<<"got write request"<<endl;
        Handler* handler = handlers[s.next];
        packet::Basic* response = handler->process(s, packet);
        if(!response)
            continue;
        sendto(sock, (const char *)response, strlen(response->data),
               MSG_CONFIRM, (const struct sockaddr *) &client_aadr,
               addr_len);
        do
        {
            timeoutExpiredCount = NUMBER_OF_FAILURES;
            do
            {
                do
                {
                    if (setsockopt(sock, SOL_SOCKET, SO_RCVTIMEO,&tv,sizeof(tv)) < 0) {
                        perror("Error");
                    }
                    // TODO: Wait WAIT_FOR_PACKET_TIMEOUT to see if something appears
                    // for us at the socket (we are waiting for DATA)
                    write_req_len = recvfrom(sock, &packet, MAX_PACK_SIZE, 0,
                                                 (struct sockaddr *) &client_aadr,
                                                 &addr_len);



                    if (write_req_len > 0)// TODO: if there was something at the socket and
                        // we are here not because of a timeout
                    {
                        // TODO: Read the DATA packet from the socket (at
                        // least we hope this is a DATA packet)
                        cout<<"got write request"<<endl;
                        response = handlers[s.next]->process(s, packet);
                    }
                    if (write_req_len < 0) // TODO: Time out expired while waiting for data
                    //to appear at the socket
                    {
                        //TODO: Send another ACK for the last packet
                        timeoutExpiredCount++;
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
            }while (timeoutExpiredCount < NUMBER_OF_FAILURES);
            // = 0;
            //lastWriteSize = fwrite(...); // write next bulk of data
            // TODO: send ACK packet to the client
            // if recieved a smaller packet the 512, finish session
            if(0) {
                session_in_progress = false;
            }
        }while (session_in_progress); // Have blocks left to be read from client (not end of transmission)
    }while (server_alive);

}

Server::Server(int port_num) {
    //init handlers
    handlers[Opcode::WRQ_OPCODE] = new WRQHandler();
    handlers[Opcode::DATA_OPCODE] = new DataHandler();
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
    server_alive = false;



}
