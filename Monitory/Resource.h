//
// Created by zuznanna on 15.05.2020.
//

#ifndef SEMAFORY_WATKI_RESOURCE_H
#define SEMAFORY_WATKI_RESOURCE_H

#include <list>
#include <semaphore.h>
#include "monitor.h"
#include "Message.h"
#include "Random.h"

using namespace std;

class Resource : public Monitor {
private:
    list<Message*> queue;
    int idQueue;
    int maxLengthQueue;
    int actLengthQueue;

    Condition blockWhenEmpty;
    Condition blockWhenFull;

public:
    Resource(int length, int id);
    void produce(Message *m);
    void consume(int idConsumer);
    void read(int idReader);
};


#endif //SEMAFORY_WATKI_RESOURCE_H
