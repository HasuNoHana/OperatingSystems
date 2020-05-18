//
// Created by zuznanna on 15.05.2020.
//

#ifndef SEMAFORY_WATKI_CONSUMER_H
#define SEMAFORY_WATKI_CONSUMER_H

#include <chrono>
#include <thread>
#include "Resource_guard.h"

class Consumer {
private:
    int idConsumer;
public:
    Consumer(int idConsumer);

    [[noreturn]] void consume(Resource_guard *resourceGuard);

};


#endif //SEMAFORY_WATKI_CONSUMER_H
