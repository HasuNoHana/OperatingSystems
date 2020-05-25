//
// Created by zuznanna on 15.05.2020.
//

#ifndef SEMAFORY_WATKI_MESSAGE_H
#define SEMAFORY_WATKI_MESSAGE_H
#include <iostream>

class Message {
private:
    int idQueue;
    int idProducer;
    int idMessage;

public:
    Message(int idQueue, int idProducer, int idMessage);
    friend std::ostream& operator<<(std::ostream& os, Message *m);

    int getIdQueue() const;

    int getIdProducer() const;

    int getIdMessage() const;
};



#endif //SEMAFORY_WATKI_MESSAGE_H
