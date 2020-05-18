//
// Created by zuznanna on 15.05.2020.
//

#ifndef SEMAFORY_WATKI_RESOURCE_H
#define SEMAFORY_WATKI_RESOURCE_H

#include <list>
#include <semaphore.h>
#include "Message.h"
#include "Random.h"

using namespace std;

class Resource {
private:
    list<Message*> queue;
    int idQueue;
    int numberOfReaders;
    //0 - semaphore is blocked
    sem_t stepInside;
    sem_t blockWhenEmpty;
    sem_t blockWhenFull;
    sem_t countReaders;

public:
    Resource(int length, int id, int numberOfReaders);
    void produce(Message *m);
    void consume(int idConsumer);
    void read(int idReader);
};


#endif //SEMAFORY_WATKI_RESOURCE_H
