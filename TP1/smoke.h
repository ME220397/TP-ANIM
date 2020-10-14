#ifndef SMOKE_H
#define SMOKE_H

#include <iostream>
#include <list>
#include <QVector3D>
#include "puff.h"

using namespace std;
class Smoke
{
public:
    Smoke(QVector3D position, float timeInterval);
    void addPuff(Puff puff);
    void animate(float dt);

private:
    QVector3D position;
    list<Puff> puffsList;
    float timeInterval;
    float elapsedTime;

};

#endif // SMOKE_H
