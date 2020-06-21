//
// Created by daniel on 20/06/2020.
//

#include <iostream>
#include <netinet/in.h>
#include <cstring>
#include "wrq_handler.h"

using namespace std;

STATUS WRQHandler::process(State &s, packet::Basic &p,packet::Ack & out_pack) {
    p.opcode = ntohs(p.opcode);
    if(p.opcode != Opcode::WRQ_OPCODE){
        cout<<"not the right opcode"<<endl;
        return STATUS::OP_CODE_ERROR;
    }
    char name[MAX_PACK_SIZE];
    char protocol [MAX_PACK_SIZE];
    //cout<<ntohs(p.opcode)<<endl;
    p.data[MAX_PACK_SIZE-3]='\0';
    strcpy(name, p.data);
    strcpy(protocol, (p.data + strlen(name) + 1));
    cout<<"IN: WRQ,"<<name<<", "<<protocol<<endl;

    FILE* fd = fopen(name,"w+");
    if(fd == nullptr){
        cout<<"file didnt open";
        return STATUS::FILE_WRITE_ERROR;
    }
    fclose(fd);
    s.next = DATA_OPCODE;
    out_pack.opcode = htons(Opcode::ACK_OPCODE);
    out_pack.block_number = htons(s.ack_num);
    return STATUS::OK;
}
