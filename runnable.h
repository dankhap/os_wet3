//
// Created by daniel on 24/05/2020.
//

#ifndef HW2_RUNNABLE_H
#define HW2_RUNNABLE_H


#include <sys/types.h>
#include <pthread.h>

class Runnable {

protected:
    pthread_t t;

public:
    Runnable():t(){}
    virtual void run() = 0;
    void start();
    void join();


private:
    static void* start_inner(void* r){
        ((Runnable*)r)->run();
        return nullptr;
    }

};


#endif //HW2_RUNNABLE_H
