//
// Created by zuznanna on 15.05.2020.
//

#ifndef SEMAFORY_WATKI_RANDOM_H
#define SEMAFORY_WATKI_RANDOM_H
#include <random>

#define MINIMAL_TIME_SLEEP 10
#define MAXIMAL_TIME_SLEEP 100
#define PROGRAM_TIME 10

class Random {
public:
    static int getRandomNumber(const int & min, const int & max);
};


#endif //SEMAFORY_WATKI_RANDOM_H
