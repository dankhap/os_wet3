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
    data_pack.opcode = ntohs( data_pack.opcode);
    if(data_pack.opcode != Opcode::DATA_OPCODE){
        cout<<"wrong OP"<<endl;
        return STATUS::OP_CODE_ERROR;
    }
    uint16_t block_number = ntohs(data_pack.block_number);
    if(!s.checkBlock(block_number)){
        cout<<"wrong block"<<endl;
        return STATUS::BLOCK_NUM_ERROR;
    }
    cout<<"IN: DATA,"<<block_number<<", "<<s.curr_pack_len<<endl;
    s.ack_num+=1;
    out_pack.opcode = htons(Opcode::ACK_OPCODE);
    out_pack.block_number = htons(s.ack_num);
    if(s.curr_pack_len < MAX_PACK_SIZE){
        return STATUS::LAST_PACK;
    }

    return STATUS::OK;
}
