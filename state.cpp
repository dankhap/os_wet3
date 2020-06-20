//
// Created by daniel on 20/06/2020.
//

#include "state.h"

State::State() :next(Opcode::WRQ_OPCODE), next_block(0), data(nullptr),curr_pack_len(0){

}

int State::reset() {
    next = Opcode::WRQ_OPCODE;
    next_block = 0;
    if(data != nullptr)
        delete data;
    filename="";
    return 1;
}

bool State::checkBlock(int block_num) {
    if(block_num != next_block +1){
        return false;
    }
    next_block+=1;
    return true;
}
