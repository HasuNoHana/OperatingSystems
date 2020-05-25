//
// Created by zuznanna on 15.05.2020.
//


#include "Random.h"


int Random::getRandomNumber(const int &min, const int &max) {
    static thread_local std::mt19937 generator;
    std::uniform_int_distribution<int> distribution(min,max);
    return distribution(generator);
}
