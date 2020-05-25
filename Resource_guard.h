//
// Created by zuznanna on 15.05.2020.
//

#ifndef SEMAFORY_WATKI_RESOURCE_GUARD_H
#define SEMAFORY_WATKI_RESOURCE_GUARD_H


#include "Resource.h"

class Resource_guard {
private:
    Resource *queue1;
    Resource *queue2;

public:

    Resource_guard(int length);
    void produce(Message *m);
    void consume(int idConsumer);
    void read(int idQueue, int idReader);
};


#endif //SEMAFORY_WATKI_RESOURCE_GUARD_H
