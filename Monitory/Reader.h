//
// Created by zuznanna on 15.05.2020.
//

#ifndef SEMAFORY_WATKI_READER_H
#define SEMAFORY_WATKI_READER_H

#include <chrono>
#include <thread>
#include "Resource_guard.h"



class Reader {
private:
    int idQueue;
    int idReader;
public:
    Reader(int idQueue, int idReader);

    [[noreturn]] void read(Resource_guard *resourceGuard);//noreturn jest tam while true więc nie kończy się
};


#endif //SEMAFORY_WATKI_READER_H
