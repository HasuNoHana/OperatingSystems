//
// Created by zuznanna on 15.05.2020.
//

#ifndef SEMAFORY_WATKI_PRODUCER_H
#define SEMAFORY_WATKI_PRODUCER_H

#include <chrono>
#include <thread>
#include "Resource_guard.h"

class Producer {
private:
    int idQueue;
    int idProducer;
public:
    Producer(int idQueue, int idProducer);

    [[noreturn]] void produce(Resource_guard *resourceGuard);//noreturn jest tam while true więc nie kończy się
};


#endif //SEMAFORY_WATKI_PRODUCER_H
