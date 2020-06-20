//
// Created by daniel on 11/06/2020.
//
#include "server.h"
#include <iostream>

using std::cout;
using std::endl;

int main(int argc , char ** argv){
    if(argc != 2){
        cout<<" invalid number of arguments"<<endl;
    }


    Server s((uint16_t)std::stoi(argv[1]));
    s.run();
}