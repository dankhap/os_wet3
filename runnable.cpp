//
// Created by daniel on 24/05/2020.
//

#include "runnable.h"

void Runnable::start() {
    pthread_create(&t, nullptr, Runnable::start_inner, (void*)this);

}

void Runnable::join() {
    pthread_join(t,nullptr);

}
