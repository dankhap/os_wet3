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

    p.data[MAX_PACK_SIZE-3]='\0';
    string name(p.data);
    string protocol(p.data + name.length() + 1);
    cout<<"IN: WRQ,"<< name<<", "<<protocol<<endl;
    if(protocol != "octet"){
        cout << "not supported protocol:" << protocol << endl;
        return STATUS::FILE_WRITE_ERROR;
    }
    FILE* fd = fopen(name.c_str(),"w+");
    if(fd == nullptr){
        cout<<"file didnt open";
        return STATUS::FILE_WRITE_ERROR;
    }
    fclose(fd);
    s.next = DATA_OPCODE;
    s.setFilename(name);
    s.setProtocol(protocol);
    out_pack.opcode = htons(Opcode::ACK_OPCODE);
    out_pack.block_number = htons(s.ack_num);

    return STATUS::OK;
}
