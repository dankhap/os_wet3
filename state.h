//
// Created by daniel on 20/06/2020.
//

#ifndef TTFTPS_STATE_H
#define TTFTPS_STATE_H

#include <string>
#include "packets.h"

class State {
public:
    State();
    int reset();
    Opcode next;

private:
    unsigned int next_block;
    std::string filename;
    std::string protocol;
    char* data;
};


#endif //TTFTPS_STATE_H
