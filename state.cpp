//
// Created by daniel on 20/06/2020.
//

#include "state.h"

State::State() :next(Opcode::WRQ_OPCODE), next_block(0), data(nullptr){

}

int State::reset() {
    next = Opcode::WRQ_OPCODE;
    next_block = 0;
    if(data != nullptr)
        delete data;
    filename=""
}
