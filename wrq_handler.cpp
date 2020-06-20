//
// Created by daniel on 20/06/2020.
//

#include <iostream>
#include <netinet/in.h>
#include <cstring>
#include "wrq_handler.h"

using namespace std;

packet::Basic *WRQHandler::process(State &s, packet::Basic &p) {
    if(p.opcode != Opcode::WRQ_OPCODE)
        return nullptr;
    char name[MAX_PACK_SIZE];
    char protocol [MAX_PACK_SIZE];

    cout<<ntohs(p.opcode)<<endl;
    p.data[MAX_PACK_SIZE-3]='\0';
    strcpy(name, p.data);
    strcpy(protocol, (p.data + strlen(name) + 1));
    cout<<name<<" "<<protocol<<" "<<endl;
    exit(1);
}
