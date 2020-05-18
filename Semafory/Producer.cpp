//
// Created by zuznanna on 15.05.2020.
//

#include <iostream>
#include "Producer.h"

Producer::Producer(int idQueue, int idProducer) : idQueue(idQueue), idProducer(idProducer) {}

[[noreturn]] void Producer::produce(Resource_guard *resourceGuard) {
    int timer=0;
    int currentMessageId=0;
    while (true) {
        int random = Random::getRandomNumber(MINIMAL_TIME_SLEEP, MAXIMAL_TIME_SLEEP);
        //timer++;

        Message *m = new Message(idQueue, idProducer, currentMessageId);
        currentMessageId++;
        resourceGuard->produce(m);
        std::this_thread::sleep_for(std::chrono::milliseconds(random));
    }
}
