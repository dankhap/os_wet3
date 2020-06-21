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

    STATUS state = {OK};
    unsigned int curr_pack_len;
    Opcode next;
    bool checkBlock(uint16_t block_num);
    int ack_num;

private:
    unsigned int next_block;
    std::string filename;
public:
    const std::string &getFilename() const;

    void setFilename(const std::string &filename);

    const std::string &getProtocol() const;

    void setProtocol(const std::string &protocol);

private:
    std::string protocol;
    char* data;
};


#endif //TTFTPS_STATE_H
