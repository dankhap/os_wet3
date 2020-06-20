//
// Created by daniel on 20/06/2020.
//

#ifndef TTFTPS_HANDLER_H
#define TTFTPS_HANDLER_H


#include "packets.h"
#include "state.h"

class Handler {
public:
    virtual STATUS process(State& s, packet::Basic& packet,packet::Ack& out_pack) = 0;

};


#endif //TTFTPS_HANDLER_H
