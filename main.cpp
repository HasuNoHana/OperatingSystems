#include <iostream>
#include <thread>
#include "Resource_guard.h"
#include "Producer.h"
#include "Consumer.h"
#include "Reader.h"

#define QUEUE_LENGTH 10

using namespace std;

int main() {
    auto *resourceGuard = new Resource_guard(QUEUE_LENGTH);

    thread tp1(&Producer::produce, Producer(1,1), resourceGuard);
    thread tp2(&Producer::produce, Producer(2,2), resourceGuard);
    thread tc1(&Consumer::consume, Consumer(1), resourceGuard);
    thread tr1(&Reader::read, Reader(1,1), resourceGuard);

    tp1.join();
    tp2.join();
    tc1.join();
    tr1.join();

    return 0;
}
