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
    bool session_in_progress = false;

    server_alive = true;
    int timeoutExpiredCount = 0;
    fd_set readset;
    int result = 0;
    do
    {
        // establish connection with client and get WRQ packet
        socklen_t addr_len = sizeof(client_aadr);
        packet::Basic packet = {0};
        packet::Ack ack(0);
        FD_ZERO(&readset);
        FD_SET(sock, &readset);
        if(-1 == select(sock + 1, &readset, nullptr, nullptr, nullptr))
        {
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
        session_in_progress = true;
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
                        session_in_progress = false;
                        break;
                    }
                    // Continue while some socket was ready
                    //but recvfrom somehow failed to read the data
                }while (s.curr_pack_len <= 0 && s.state != STATUS::TIMEOUT_ERROR) ;

                if (s.state == STATUS::OP_CODE_ERROR) // We got something else but DATA
                {
                    PRINT_ERROR_OPCODE();
                    session_in_progress = false;
                    goto NEXTSESS;
                    // FATAL ERROR BAIL OUT
                }
                if (s.state == STATUS::BLOCK_NUM_ERROR)
                     // The incoming block number is not what we have
                     //expected, i.e. this is a DATA pkt but the block number
                     //in DATA was wrong (not last ACK’s block number + 1)
                {
                    PRINT_ERROR_BLOCK();
                    session_in_progress = false;
                    goto NEXTSESS;
                     //FATAL ERROR BAIL OUT
                }
            }while (timeoutExpiredCount < NUMBER_OF_FAILURES && (s.state!= STATUS::OK && s.state!= STATUS::LAST_PACK));
            if(s.state == LAST_PACK){
                session_in_progress = false;
            }
            if(s.state == STATUS::TIMEOUT_ERROR){
                PRINT_ERROR_TIMEOUT();
                session_in_progress = false;
                goto NEXTSESS;
            }
            sendto(sock, (const char *)&ack, sizeof(ack),
                   MSG_CONFIRM, (const struct sockaddr *) &client_aadr,
                   addr_len);
            printACK(ack);
            NEXTSESS:;
        }while (session_in_progress); // Have blocks left to be read from client (not end of transmission)
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
        cout<<errno;
        exit(1);
    }
    // initilize server struct
    memset(&server_addr,0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    server_addr.sin_port = htons(port_num);
    int res = bind(sock,(struct sockaddr*) &server_addr, sizeof(server_addr));
    if( res < 0 ){
        cout<<"something wrong in bind, err:" << errno << endl;
        exit(1);
    }
    server_alive = false;


}

void Server::printACK(packet::Ack pack) {
    cout<<"OUT:ACK, "<<ntohs(pack.block_number)<<endl;
}



int Server::writeTofile(packet::Basic &packet) {
    packet::DataPack data_pack = reinterpret_cast<const packet::Data &>(packet);
    int size_of_data = s.curr_pack_len - 4;
    cout<<"size of last data is "<< size_of_data<<endl;
    FILE* fd = fopen(s.getFilename().c_str(), "a");
    int size_written = fwrite((void*)&data_pack.data,1, size_of_data, fd);
    fclose(fd);
    cout<<"WRITING: "<<size_written<<endl;
    return size_written;
}

void Server::PRINT_ERROR_OPCODE() {
    cout<<"FLOWERROR: unexpected opcode arrived, termination connection..."<<endl;

}

void Server::PRINT_ERROR_BLOCK() {
    cout<<"FLOWERROR: unexpected block number arrived, termination connection..."<<endl;
}

void Server::PRINT_ERROR_TIMEOUT() {
    cout<<"FLOWERROR: Time out occured for the 7th time, termination connection..."<<endl;
}
