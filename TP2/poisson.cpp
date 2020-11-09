#include "poisson.h"
#include <QtMath>
#include<QRandomGenerator>
Poisson::Poisson(QVector3D position, float taille, QVector3D vitesse){
    this->position = position;
    this->taille = taille;
    this->vitesse = vitesse;
    this->angle = 0;
    this->time = 0.0;
    this->speed_changed = false;
    this->dist_perimeter=5.f;
    this->flocking_rotation.rotate(90, 0,1,0);

}

// Getters and Setters
void Poisson::set_program(QOpenGLShaderProgram *program){
    program_poisson = program;
}
QVector3D Poisson::get_position(){
    return position;
}
float Poisson::get_size(){
    return taille;
}
QVector3D Poisson::get_speed(){
    return vitesse;
}
float Poisson::get_dist_perimeter(){
    return dist_perimeter;
}

void Poisson::set_position(QVector3D position){
    this->position = position;
}
void Poisson::set_size(float taille){
    this->taille = taille;
}
void Poisson::set_speed(QVector3D vitesse){
    this->vitesse = vitesse;
}

// Function

void Poisson::anime(float dt, int w, int h, int d){
    int x,y,z;
    position = position + vitesse * dt;

    x= position[0];
    y= position[1];
    z= position[2];

    if(x>w || x < -w){
        position.setX(-x);
    }
    if(y>h || y < -h){
        position.setY(-y);
    }
    if(z>d || z < -d){
        position.setZ(-z);
    }

}

void Poisson::affiche(QMatrix4x4 projectionMatrix, QMatrix4x4 viewMatrix){
    program_poisson->bind();

    QMatrix4x4 modelMatrixPoisson;
    modelMatrixPoisson.translate(position);
    modelMatrixPoisson *= flocking_rotation;


    program_poisson->setUniformValue("projectionMatrix", projectionMatrix);
    program_poisson->setUniformValue("viewMatrix", viewMatrix);
    program_poisson->setUniformValue("modelMatrix", modelMatrixPoisson);

    program_poisson->setUniformValue("particleSize", taille);
    program_poisson->setAttributeBuffer("in_position", GL_FLOAT ,0, 3,  6*sizeof(GLfloat));
    program_poisson->setAttributeBuffer("in_color", GL_FLOAT, 3*sizeof (GLfloat), 3, 6*sizeof(GLfloat));

    program_poisson->enableAttributeArray("in_position");
    program_poisson->enableAttributeArray("in_color");

    glDrawArrays(GL_TRIANGLES, 0, 9);

    program_poisson->disableAttributeArray("in_position");
    program_poisson->disableAttributeArray("in_color");

    program_poisson->release();
}

bool Poisson::dans_voisinage(QVector3D p){
    float dist = position.distanceToPoint(p);
    if(dist <= dist_perimeter)
        return true;
    return false;
}

