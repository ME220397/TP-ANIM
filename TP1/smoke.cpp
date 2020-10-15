#include "smoke.h"
#include <QRandomGenerator>

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

int rand_life_time(int min, int max){
    QRandomGenerator rand;
    return rand.bounded(min, max+1);
}

Puff * Smoke::get_last_puff(){
    return &puffsList.back();
}

void Smoke::remove_disappeared_puffs(){
    list<Puff>::iterator i;
    i = puffsList.begin();
    while(i != puffsList.end()){
        if(i->get_remaining_time() <=0)
            i = puffsList.erase(i);
        else
            i++;
    }
}

void Smoke::calc_new_position(float dt){
    list<Puff>::iterator i;
    i = puffsList.begin();
    while(i!=puffsList.end()){
        i->animate(dt);
        i++;
    }
}

void Smoke::animate(float dt){
    //Etape 1
    elapsedTime += dt;
    if( elapsedTime > timeInterval){
        elapsedTime = 0;
        float remaining_time = (float) rand_life_time(5, 8);
        float size = get_last_puff()->get_size();
        QVector3D pos_init = position;
        QVector3D vitesse = QVector3D(0, 1, 0);
        Puff p(pos_init, size, vitesse, remaining_time);
        addPuff(p);
    }
    //Etape 2
    remove_disappeared_puffs();
    //Etape 3
    calc_new_position(dt);
}

void Smoke::set_particle(QOpenGLShaderProgram *program_particle){
    this->program_particule = program_particle;
}

void Smoke::set_texture(QOpenGLTexture *texture){
    this->texture = texture;
}


void Smoke::display(){
    list<Puff>::iterator i;
    i = puffsList.begin();
    while(i!=puffsList.end()){
        i->set_texture(texture);
        i->set_particle(program_particule);
        i->display();
        i++;
    }
}
