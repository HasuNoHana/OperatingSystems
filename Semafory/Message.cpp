//
// Created by zuznanna on 15.05.2020.
//

#include "Message.h"

Message::Message(int idQueue, int idProducer, int idMessage) : idQueue(idQueue), idProducer(idProducer), idMessage(idMessage) {}

std::ostream& operator<<(std::ostream& os, Message *m)
{
    os <<"Wiadomosc o id: "<< m->getIdMessage() << " kolejka: " << m->getIdQueue() << " producer: " << m->getIdProducer()<<std::endl;
    return os;
}

int Message::getIdQueue() const {
    return idQueue;
}

int Message::getIdProducer() const {
    return idProducer;
}

int Message::getIdMessage() const {
    return idMessage;
}
