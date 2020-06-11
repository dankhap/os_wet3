//
// Created by daniel on 11/06/2020.
//

#ifndef OS_WET3_SERVER_H
#define OS_WET3_SERVER_H

#include "runnable.h"
#include <string>
class Server : public Runnable{

public:
    explicit Server(std::string hostname);
    void run() override;
};


#endif //OS_WET3_SERVER_H

