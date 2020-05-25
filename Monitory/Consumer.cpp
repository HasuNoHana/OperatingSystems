//
// Created by zuznanna on 15.05.2020.
//

#include "Consumer.h"

Consumer::Consumer(int idConsumer) : idConsumer(idConsumer) {}

[[noreturn]] void Consumer::consume(Resource_guard *resourceGuard) {
    while (true) {
        int random = Random::getRandomNumber(MINIMAL_TIME_SLEEP, MAXIMAL_TIME_SLEEP);
        resourceGuard->consume(idConsumer);
        std::this_thread::sleep_for(std::chrono::milliseconds(random));
    }
}
