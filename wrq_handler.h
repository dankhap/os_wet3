//
// Created by daniel on 20/06/2020.
//

#ifndef TTFTPS_WRQ_HANDLER_H
#define TTFTPS_WRQ_HANDLER_H


#include "handler.h"

class WRQHandler : public Handler {
    packet::Ack * process(State& s, packet::Basic& packet) override;

};


#endif //TTFTPS_WRQ_HANDLER_H
