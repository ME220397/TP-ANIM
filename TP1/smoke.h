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
    void display();
    void set_particle(QOpenGLShaderProgram *program_particle);
    void set_texture(QOpenGLTexture *texture);
    void translate_position(float x, float y, float z);

private:
    QVector3D position;
    list<Puff> puffsList;
    float timeInterval;
    float elapsedTime;
    Puff * get_last_puff();
    void remove_disappeared_puffs();
    void calc_new_position(float dt);
    QOpenGLShaderProgram *program_particule;
    QOpenGLTexture *texture;
    int number_of_rows;
};

#endif // SMOKE_H
