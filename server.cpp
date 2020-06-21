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
        s.curr_pack_len = recvfrom(sock, &packet, MAX_PACK_SIZE, 0,
                (struct sockaddr *) &client_aadr,
                &addr_len);
        if(s.curr_pack_len <= 0)
            continue;
        s.state = handlers[s.next]->process(s, packet,out);
        if(s.state != STATUS::OK)
            continue;
        sendto(sock, (const char *)&out, sizeof(out),
               MSG_CONFIRM, (const struct sockaddr *) &client_aadr,
               addr_len);
        printACK(out);
        do
        {
            do
            {
                do
                {
                    if (setsockopt(sock, SOL_SOCKET, SO_RCVTIMEO,&tv,sizeof(tv)) < 0) {
                        perror("Error");
                    }
                    // TODO: Wait WAIT_FOR_PACKET_TIMEOUT to see if something appears
                    // for us at the socket (we are waiting for DATA)
                    s.curr_pack_len = recvfrom(sock, &packet, MAX_PACK_SIZE, 0,
                                                 (struct sockaddr *) &client_aadr,
                                                 &addr_len);

                    if (s.curr_pack_len > 0)// TODO: if there was something at the socket and
                        // we are here not because of a timeout
                    {
                        // TODO: Read the DATA packet from the socket (at
                        // least we hope this is a DATA packet)
                        s.state = handlers[s.next]->process(s, packet,out);
                    }
                    if (s.curr_pack_len < 0) // TODO: Time out expired while waiting for data
                    //to appear at the socket
                    {
                        //TODO: Send another ACK for the last packet
                        timeoutExpiredCount++;
                    }
                    if (timeoutExpiredCount>=NUMBER_OF_FAILURES)//timeoutExpiredCount>= NUMBER_OF_FAILURES)
                    {
                        // FATAL ERROR BAIL OUT
                    }
                }while (s.curr_pack_len < 0) ;// TODO: Continue while some socket was ready
                  //but recvfrom somehow failed to read the data
                if (s.state == STATUS::OP_CODE_ERROR) // TODO: We got something else but DATA
                {
                    // FATAL ERROR BAIL OUT
                }
                if (s.state == STATUS::BLOCK_NUM_ERROR) // TODO: The incoming block number is not what we have
                 //expected, i.e. this is a DATA pkt but the block number
                 //in DATA was wrong (not last ACK’s block number + 1)
                {
                     //FATAL ERROR BAIL OUT
                }
            }while (timeoutExpiredCount < NUMBER_OF_FAILURES && (s.state!= STATUS::OK || s.state!= STATUS::LAST_PACK));
            if(s.state == LAST_PACK){
                session_in_progress = false;
            }
            writeTofile(packet);
            sendto(sock, (const char *)&out, sizeof(out),
                   MSG_CONFIRM, (const struct sockaddr *) &client_aadr,
                   addr_len);
            printACK(out);
        }while (session_in_progress); // Have blocks left to be read from client (not end of transmission)
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
