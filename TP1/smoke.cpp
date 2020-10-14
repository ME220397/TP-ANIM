#include "smoke.h"

Smoke::Smoke(QVector3D position, float timeInterval)
{
    this->position = position;
    this->timeInterval = timeInterval;
    this->elapsedTime = 0;
    this->puffsList = list<Puff>();
}

void Smoke::addPuff(Puff puff){
    puffsList.push_back(puff);
}

void Smoke::animate(float dt){
    //Etape 1
    //Etape 2
    //Etape 3
}
