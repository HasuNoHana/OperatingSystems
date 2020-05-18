//
// Created by zuznanna on 15.05.2020.
//


#include "Reader.h"

Reader::Reader(int idQueue, int idReader) : idQueue(idQueue), idReader(idReader) {}

[[noreturn]] void Reader::read(Resource_guard *resourceGuard) {

    int timer=0;
    while (true) {
        int random = Random::getRandomNumber(MINIMAL_TIME_SLEEP, MAXIMAL_TIME_SLEEP);
        //timer++;
        resourceGuard->read(idQueue, idReader);
        std::this_thread::sleep_for(std::chrono::milliseconds(random));
    }
}
