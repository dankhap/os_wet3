//
// Created by daniel on 20/06/2020.
//

#ifndef TTFTPS_HANDLER_H
#define TTFTPS_HANDLER_H


#include "packets.h"
#include "state.h"

class Handler {
public:
    virtual packet::Ack* process(State& s, packet::Basic& packet) = 0;

};


#endif //TTFTPS_HANDLER_H
