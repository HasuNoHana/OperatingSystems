//
// Created by zuznanna on 15.05.2020.
//

#include "Resource.h"

Resource::Resource(int length, int id) {
    idQueue = id;
    maxLengthQueue = length;
    actLengthQueue = 0;
}

void Resource::produce(Message *m) {
    enter();    //wejdz do sekcji krytycznej
    if(actLengthQueue == maxLengthQueue)    //sprawdz czy kolejka jest pełna
        wait(blockWhenFull);    //jest pełna to wychodzi z sekcji krytycznej i czeka az nie bedzie pelna

    actLengthQueue++;   //aktualizuj obecna dlugosc kolejki
    queue.push_back(m);
    cout<<"Producer "<<m->getIdProducer()<<" added message "<<m->getIdMessage()<<" to queue "<<idQueue<<endl;

    if(actLengthQueue == 1) //jesli jest to pierwszy element kolejki odblokuj żeby pozostale procesy mogly pracowac
        signal(blockWhenEmpty);
    leave();
}

void Resource::consume(int idConsumer) {
    enter();
    if(actLengthQueue == 0) //jesli kolejka jest pusta to czekaj az nie bedzie
        wait(blockWhenEmpty);

    actLengthQueue--;   //aktualizuj obecna dlugosc kolejki
    int random = Random::getRandomNumber(0,queue.size()-1);
    for (auto i = queue.begin(); i != queue.end();) {
        if (random == 0) {
            i = queue.erase(i);
            break;
        }
        random --;
    }
    cout<<"Consumer "<<idConsumer<<" deleted message in queue "<<idQueue<<endl;

    if(actLengthQueue == maxLengthQueue-1)  //jesli kolejka wlasnie przestala byc pelna to zaznacz to
        signal(blockWhenFull);
    leave();
}

void Resource::read(int idReader) {
    enter();

    cout << "Reader "<<idReader<<" is reading messages from queue " << idQueue << endl;
    for (auto m : queue)
        cout << m;
    cout << endl;

    leave();
}
