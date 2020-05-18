//
// Created by zuznanna on 15.05.2020.
//

#include "Resource.h"

Resource::Resource(int length, int id, int numberOfReaders) {
    idQueue = id;
    numberOfReaders = numberOfReaders;
    sem_init(&blockWhenEmpty, 0, 0);
    sem_init(&blockWhenFull, 0, length);
    sem_init(&stepInside, 0, 1);
    sem_init(&countReaders, 0, 1);
}

void Resource::produce(Message *m) {
    sem_wait(&blockWhenFull);   //it cant add when there is no place to add
    sem_wait(&stepInside);

    queue.push_back(m);
    cout<<"Producer "<<m->getIdProducer()<<" added message "<<m->getIdMessage()<<" to queue "<<idQueue<<endl;

    sem_post(&stepInside);
    sem_post(&blockWhenEmpty);    //unlock(increment) full
}

void Resource::consume(int idConsumer) {
    sem_wait(&blockWhenEmpty);//it cant consume when there is nothing to consume
    sem_wait(&stepInside);

    auto *r = new Random();
    int random = r->getRandomNumber(0,queue.size()-1);

    for (auto i = queue.begin(); i != queue.end();) {
        if (random == 0) {
            i = queue.erase(i);
            break;
        }
        random --;
    }
    cout<<"Consumer "<<idConsumer<<" deleted message in queue "<<idQueue<<endl;

    sem_post(&stepInside);
    sem_post(&blockWhenFull);//it is less full now
}

void Resource::read(int idReader) {
//  if you don't undastand why read about Readers-Writers Problem
    sem_wait(&countReaders);//I block countReaders because I want to increment numberOfReaders
    numberOfReaders++;
    if(numberOfReaders==1)//if I'm the fist reader I block step inside from other types of proceses
        sem_wait(&stepInside);
    sem_post(&countReaders);//I unblock countReaders because Other readers also can come inside

    cout << "Reader "<<idReader<<" is reading messages from queue " << idQueue << endl;
    for (auto m : queue)
        cout << m;
    cout << endl;

    sem_wait(&countReaders);//I block countReaders because I want to decrement numberOfReaders
    numberOfReaders--;
    if(numberOfReaders==0)
        sem_post(&stepInside);//if I'm the last reader I free step inside
    sem_post(&countReaders);
}
