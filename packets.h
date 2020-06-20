//
// Created by os on 19.6.2020.
//

#ifndef TTFTPS_PACKETS_H
#define TTFTPS_PACKETS_H

#include <cstdint>
#include <string>

enum Opcode {
    WRQ_OPCODE=2,
    DATA_OPCODE=3,
    ACK_OPCODE=4,
    ERR_OPCODE=5
};

typedef enum STATUS_t{
    OK = 0,
    OP_CODE_ERROR,
    BLOCK_NUM_ERROR,
    LAST_PACK,
}STATUS;

#define MAX_PACK_SIZE 516
namespace packet {
    typedef struct Basic {
        uint16_t opcode;
        char data[MAX_PACK_SIZE - 2];
    }__attribute__((packed)) Basic;

    typedef struct Ack {
        Ack(uint16_t b_number): block_number(b_number){}
        uint16_t opcode=ACK_OPCODE;
        uint16_t block_number{};
    }__attribute__((packed)) Ack;

    typedef struct Data {
        uint16_t opcode;
        uint16_t block_number;
        uint8_t data[MAX_PACK_SIZE - 4];
    }__attribute__((packed)) DataPack;


    struct WRQ {
        uint16_t opcode;
        std::string filename;
        std::string mode;
    };

}
#endif //TTFTPS_PACKETS_H
