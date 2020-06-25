//
// Created by daniel on 11/06/2020.
//

#include <iostream>
#include <cstring>
#include <sys/select.h>
#include "server.h"
#include "data_handler.h"
#include "wrq_handler.h"


using std::cout;
using std::endl;

void Server::run() {

    const int WAIT_FOR_PACKET_TIMEOUT = 3;
    const int NUMBER_OF_FAILURES = 7;
    server_alive = true;
    fd_set readset;
    int result = 0;

    int res = bind(sock,(struct sockaddr*) &server_addr, sizeof(server_addr));
    if( res < 0 ){
        cout<<"TTFTP_ERROR: ";
        cout<<"something wrong in bind, err:" << errno << endl;
        return;
    }

    do
    {
        // establish connection with client and get WRQ packet
        socklen_t addr_len = sizeof(client_aadr);
        packet::Basic packet = {0};
        packet::Ack ack(0);
        bool bad_session = false;
        int timeoutExpiredCount = 0;

        FD_ZERO(&readset);
        FD_SET(sock, &readset);
        if(-1 == select(sock + 1, &readset, nullptr, nullptr, nullptr))
        {
            cout<<"TTFTP_ERROR: ";
            cout << "failed to select, server closes, errno:" << errno << endl;
            exit(1);
        }
        s.curr_pack_len = recvfrom(sock, &packet, MAX_PACK_SIZE, 0,
                (struct sockaddr *) &client_aadr,
                &addr_len);
        if(s.curr_pack_len <= 0)
            continue;
        s.state = handlers[s.next]->process(s, packet, ack);
        if(s.state != STATUS::OK)
            continue;
        sendto(sock, (const char *)&ack, sizeof(ack),
               MSG_CONFIRM, (const struct sockaddr *) &client_aadr,
               addr_len);
        printACK(ack);
        do
        {
            do
            {
                do
                {
                    struct timeval tv{};
                    tv.tv_sec = WAIT_FOR_PACKET_TIMEOUT;
                    tv.tv_usec = 0;
                    s.curr_pack_len = 0;
                    FD_ZERO(&readset);
                    FD_SET(sock, &readset);
                    // Wait WAIT_FOR_PACKET_TIMEOUT to see if something appears
                    // for us at the socket (we are waiting for DATA)
                    result = select(sock + 1, &readset, nullptr, nullptr, &tv);

                    // if there was something at the socket and
                    // we are here not because of a timeout
                    if (FD_ISSET(sock, &readset))
                    {
                        // Read the DATA packet from the socket (at
                        // least we hope this is a DATA packet)
                        s.curr_pack_len = recvfrom(sock, &packet, MAX_PACK_SIZE, 0,
                                                   (struct sockaddr *) &client_aadr,
                                                   &addr_len);
                        s.state = handlers[s.next]->process(s, packet, ack);
                    }
                    // Time out expired while waiting for data
                    //to appear at the socket
                    if (result == 0)
                    {
                        //Send another ACK for the last packet
                        sendto(sock, &ack, sizeof(ack), 0, (struct sockaddr *) &client_aadr,
                               addr_len);
                        timeoutExpiredCount++;
                    }
                    if (timeoutExpiredCount>=NUMBER_OF_FAILURES)
                    {
                        s.state = STATUS::TIMEOUT_ERROR;
                    }
                    // Continue while some socket was ready
                    //but recvfrom somehow failed to read the data
                }while (s.curr_pack_len <= 0 && s.state != STATUS::TIMEOUT_ERROR) ;

            // retry receive if we got timeout but still less then NUMBER_OF_FAILURES
            }while (s.state != STATUS::TIMEOUT_ERROR && result == 0);
            bad_session = print_err(s.state);
            if(!bad_session) {
                sendto(sock, (const char *) &ack, sizeof(ack),
                       MSG_CONFIRM, (const struct sockaddr *) &client_aadr,
                       addr_len);
                printACK(ack);
            }
            timeoutExpiredCount = 0;
        }while (s.state == STATUS::OK); // Have blocks left to be read from client (not end of transmission)
        if(s.state == LAST_PACK) cout<<"RECVOK"<<endl;
        s.reset();
    }while (server_alive);

}

Server::Server(int port_num) {
    //init handlers
    handlers[Opcode::WRQ_OPCODE] = new WRQHandler();
    handlers[Opcode::DATA_OPCODE] = new DataHandler();
    // initilize socket
    if((sock = socket(PF_INET,SOCK_DGRAM,IPPROTO_UDP))<0){
        cout<<"TTFTP_ERROR: ";
        cout<<errno;
        exit(1);
    }
    // initilize server struct
    memset(&server_addr,0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    server_addr.sin_port = htons(port_num);
    server_alive = false;

}

void Server::printACK(packet::Ack pack) {
    cout<<"OUT:ACK, "<<ntohs(pack.block_number)<<endl;
}

int Server::print_err(STATUS status) {
    switch(status){
        case PROTOCOL_NOT_SUPPORTED:
            cout<<"WRQERROR: unsupported protocol, only octet is supported."<<endl;
            break;
        case FILE_WRITE_ERROR:
            cout<<"WRQERROR: cannot create file on server or packet too large."<<endl;
            cout<<"RECVFAIL"<<endl;
            break;
        case OP_CODE_ERROR:
            cout<<"FLOWERROR: unexpected opcode arrived, termination connection..."<<endl;
            cout<<"RECVFAIL"<<endl;
            break;
        case BLOCK_NUM_ERROR:
            cout<<"FLOWERROR: unexpected block number arrived, termination connection..."<<endl;
            cout<<"RECVFAIL"<<endl;
            break;
        case TIMEOUT_ERROR:
            cout<<"FLOWERROR: Time out occured for the 7th time, termination connection..."<<endl;
            cout<<"RECVFAIL"<<endl;
            break;
        default:
            return 0;
    }
    return 1;
}

Server::~Server() {
}
