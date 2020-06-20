//
// Created by daniel on 20/06/2020.
//

#include "data_handler.h"
#include <iostream>
#include <netinet/in.h>

using std::cout;
using std::endl;

STATUS DataHandler::process(State &s, packet::Basic &packet,packet::Ack& out_pack) {
    packet::DataPack data_pack = reinterpret_cast<const packet::Data &>(packet);

    int opcode = ntohs(packet.opcode);
    if(opcode != Opcode::DATA_OPCODE){
        cout<<"wrong OP"<<endl;
        return STATUS::ERROR;
    }
    int block_number = ntohs(data_pack.block_number);
    if(!s.checkBlock(block_number)){
        cout<<"wrong block"<<endl;
        return STATUS::ERROR;
    }
    cout<<"IN: DATA,"<<block_number<<", "<<s.curr_pack_len<<endl;

    int size_of_data = s.curr_pack_len - 4;
    int size_written = fwrite((void*)&packet,1,size_of_data,s.fd);

    cout<<"WRITING: "<<size_written;

    out_pack.block_number +=1;

    return STATUS::OK;
};
