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
    bool checkBlock(uint16_t block_num);

    STATUS state = {OK};
    int curr_pack_len;
    Opcode next;
    int ack_num;

private:
    std::string protocol;
    std::string filename;
    unsigned int next_block;

public:
    const std::string &getFilename() const;

    void setFilename(const std::string &filename);

    const std::string &getProtocol() const;

    void setProtocol(const std::string &protocol);

private:


};


#endif //TTFTPS_STATE_H
