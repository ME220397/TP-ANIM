#include "smoke.h"
#include <QRandomGenerator>

Smoke::Smoke(QVector3D position, float timeInterval)
{
    this->position = position;
    this->timeInterval = timeInterval;
    this->elapsedTime = 0;
    this->puffsList = list<Puff>();
    this->vit_puff_init = QVector3D(0, 3, 0);
    this->initial_puff_size = 2;
    this->number_of_rows = 4;
    this->inc_vit_texture = 0.4f;
    this->color = QVector3D(1.f,1.F,1.f);
}

void Smoke::set_color(QColor color){
    float x = (float)color.redF();
    float y = (float)color.greenF();
    float z = (float)color.blueF();

    this->color.setX(x);
    this->color.setY(y);
    this->color.setZ(z);
}

void Smoke::set_puff_size(float size){
    initial_puff_size = size;
}

void Smoke::set_vitesse_x(float v){
    vit_puff_init.setX(v);
}

void Smoke::set_vitesse_y(float v){
    vit_puff_init.setY(v);
}

void Smoke::set_vitesse_z(float v){
    vit_puff_init.setZ(v);
}

void Smoke::addPuff(Puff puff){
    puff.set_number_of_rows(number_of_rows);
    puff.set_inc_vit_text(inc_vit_texture);
    puff.set_color(color);
    puffsList.push_back(puff);
}

void Smoke::translate_position(float x, float y, float z){
    float x_curr = position.x();
    float y_curr = position.y();
    float z_curr = position.z();
    position.setX(x_curr + x);
    position.setY(y_curr + y);
    position.setZ(z_curr + z);
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
        i->dec_alpha();
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
        float size = initial_puff_size;
        QVector3D pos_init = position;
        QVector3D vitesse = vit_puff_init;
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

void Smoke::set_inc_vit_text(float v){
    inc_vit_texture = v;
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
