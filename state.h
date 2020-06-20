//
// Created by daniel on 20/06/2020.
//

#ifndef TTFTPS_STATE_H
#define TTFTPS_STATE_H

#include <string>
#include "packets.h"

class State {
public:
    STATUS state = {OK};
    unsigned int curr_pack_len;
    FILE* fd;
    State();
    int reset();
    Opcode next;
    bool checkBlock(uint16_t block_num);
    int ack_num;
private:
    unsigned int next_block;
    std::string filename;
    std::string protocol;
    char* data;
};


#endif //TTFTPS_STATE_H
