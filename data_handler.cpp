//
// Created by daniel on 20/06/2020.
//

#include "data_handler.h"
#include <iostream>
#include <netinet/in.h>

using namespace std;

STATUS DataHandler::process(State &s, packet::Basic &packet,packet::Ack& out_pack) {

    cout << "processing data packet size " << s.curr_pack_len << endl;
    packet::DataPack data_pack = reinterpret_cast<const packet::Data &>(packet);
    data_pack.opcode = ntohs( data_pack.opcode);
    if(data_pack.opcode != Opcode::DATA_OPCODE){
        cout<<"wrong OP"<<endl;
        return STATUS::OP_CODE_ERROR;
    }
    uint16_t block_number = ntohs(data_pack.block_number);
    if(!s.checkBlock(block_number)){
        cout<<"wrong block"<<endl;
        out_pack.block_number = htons(s.ack_num - 1);
        return STATUS::BLOCK_NUM_ERROR;
    }
    cout<<"IN: DATA,"<<block_number<<", "<<s.curr_pack_len<<endl;
    s.ack_num+=1;
    out_pack.block_number = htons(s.ack_num);
    if(s.curr_pack_len < MAX_PACK_SIZE){

        return STATUS::LAST_PACK;
    }
    int size_of_data = s.curr_pack_len - 4;
    int res = writeTofile(s.getFilename().c_str(), (void*)&data_pack.data, size_of_data);
    if(res < 0){
        out_pack.block_number = htons(s.ack_num - 1);
        return STATUS::FILE_WRITE_ERROR;
    }
    return STATUS::OK;
}

int DataHandler::writeTofile(const char* filename, void* data, int size_of_data) {
    cout<<"size of last data is "<< size_of_data<<endl;
    FILE* fd = fopen(filename, "a");
    int size_written = fwrite(data,1, size_of_data, fd);
    fclose(fd);
    cout<<"WRITING: "<<size_written<<endl;
    return size_written;
}