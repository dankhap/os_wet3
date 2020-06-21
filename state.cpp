//
// Created by daniel on 20/06/2020.
//

#include "state.h"


State::State() :curr_pack_len(0), next(Opcode::WRQ_OPCODE), ack_num(0), next_block(0){

}

int State::reset() {
    next = Opcode::WRQ_OPCODE;
    next_block = 0;
    filename="";
    protocol="";
    ack_num=0;
    return 1;
}

bool State::checkBlock(uint16_t block_num) {
    if(block_num != next_block +1){
        return false;
    }
    next_block+=1;
    return true;
}

const std::string &State::getFilename() const {
    return filename;
}

void State::setFilename(const std::string &fname) {
    State::filename = fname;
}

const std::string &State::getProtocol() const {
    return protocol;
}

void State::setProtocol(const std::string &proto) {
    State::protocol = proto;
}
