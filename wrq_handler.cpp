//
// Created by daniel on 20/06/2020.
//

#include <iostream>
#include <netinet/in.h>
#include <cstring>
#include "wrq_handler.h"

using namespace std;

packet::Ack *WRQHandler::process(State &s, packet::Basic &p) {
    p.opcode = ntohs(p.opcode);
    if(p.opcode != Opcode::WRQ_OPCODE){
        cout<<"not the right opcode"<<endl;
        return nullptr;
    }
    char name[MAX_PACK_SIZE];
    char protocol [MAX_PACK_SIZE];
    //cout<<ntohs(p.opcode)<<endl;
    p.data[MAX_PACK_SIZE-3]='\0';
    strcpy(name, p.data);
    strcpy(protocol, (p.data + strlen(name) + 1));
    cout<<"IN: WRQ,"<<name<<", "<<protocol<<endl;

    packet::Ack pack ={0};

    s.fd = fopen(name,"w+");
    s.next = DATA_OPCODE;
    return pack;
}
