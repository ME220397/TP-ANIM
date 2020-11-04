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
    void set_puff_size(float size);
    void set_vitesse_x(float v);
    void set_vitesse_y(float v);
    void set_vitesse_z(float v);
    void set_inc_vit_text(float v);
    void set_color(QColor color);
    void set_puff_life(float l);

private:
    QVector3D position;
    QVector3D vit_puff_init;
    float initial_puff_size;
    list<Puff> puffsList;
    float timeInterval;
    float elapsedTime;
    Puff * get_last_puff();
    void remove_disappeared_puffs();
    void calc_new_position(float dt);
    QOpenGLShaderProgram *program_particule;
    QOpenGLTexture *texture;
    QVector3D color;
    int number_of_rows;
    float inc_vit_texture;
    float initial_puff_life;
};

#endif // SMOKE_H
