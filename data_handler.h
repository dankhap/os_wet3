//
// Created by daniel on 20/06/2020.
//

#ifndef TTFTPS_DATA_HANDLER_H
#define TTFTPS_DATA_HANDLER_H


#include "state.h"
#include "handler.h"

class DataHandler : public Handler{
public:
    packet::Basic* process(State& s, packet::Basic& packet) override;

};


#endif //TTFTPS_DATA_HANDLER_H
