//
// Created by daniel on 20/06/2020.
//

#ifndef TTFTPS_DATA_HANDLER_H
#define TTFTPS_DATA_HANDLER_H


#include "state.h"
#include "handler.h"

class DataHandler : public Handler {
public:
    STATUS process(State &s, packet::Basic &packet, packet::Ack &out_pack) override;

private:
    static int writeTofile(const char *filename, void *data, int data_size);
};


#endif //TTFTPS_DATA_HANDLER_H
