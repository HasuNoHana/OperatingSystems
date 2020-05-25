//
// Created by zuznanna on 15.05.2020.
//

#include "Resource_guard.h"

Resource_guard::Resource_guard(int length) {
    queue1 = new Resource(length, 1);
    queue2 = new Resource(length, 2);
}

void Resource_guard::produce(Message *m) {
    if(m->getIdQueue() == 1)
        queue1->produce(m);
    else
        queue2->produce(m);
}

void Resource_guard::consume(int idConsumer) {
    queue1->consume(idConsumer);
    queue2->consume(idConsumer);
}

void Resource_guard::read(int idQueue, int idReader) {
    if(idQueue == 1)
        queue1->read(idReader);
    else
        queue2->read(idReader);
}
