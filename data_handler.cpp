//
// Created by daniel on 20/06/2020.
//

#include "data_handler.h"

packet::Ack *DataHandler::process(State &s, packet::DataPack &packet) {

    int opcode = ntohs(packet.opcode);
    if(opcode != Opcode::DATA_OPCODE){
        // need to throw error
        return nullptr;
    }
    int block_number = ntohs(packet.block_number);
    if(!s.checkBlock(block_number)){
        // need to throw apropriate error
        return nullptr;
    }
    cout<<"IN: DATA,"<<block_number<<", "<<size_written<<endl;

    int size_of_data = s.curr_pack_len - 4;
    int size_written = fwrite(((void*)packet,1,size_of_data,s.fd);

    cout<<"WRITING: "<<size_written;

    packet::Ack pack = {block_number+1};
    return pack;
};
