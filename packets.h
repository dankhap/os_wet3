//
// Created by os on 19.6.2020.
//

#ifndef TTFTPS_PACKETS_H
#define TTFTPS_PACKETS_H

#include <cstdint>
#include <cstring>

enum Opcode {
    WRQ_OPCODE=2,
    DATA_OPCODE=3,
    ACK_OPCODE=4,
    ERR_OPCODE=5
};

#define MAX_PACK_SIZE 516



typedef struct AckPack{
    uint16_t opcode;
    uint16_t block_number;

}__attribute__((packed)) AckPack;

typedef struct DataPack{
    uint16_t opcode;
    uint16_t block_number;
    uint8_t data[MAX_PACK_SIZE-4];

}__attribute__((packed)) DataPack;


typedef struct WriteRequest{
    uint16_t opcode;
    char data[MAX_PACK_SIZE-2];

}__attribute__((packed)) WriteRequest;;


#endif //TTFTPS_PACKETS_H
