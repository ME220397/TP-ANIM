#include "poisson.h"

Poisson::Poisson(QVector3D position, float taille, QVector3D vitesse){
    this->position = position;
    this->taille = taille;
    this->vitesse = vitesse;
}

// Getters and Setters

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

void Poisson::anime(float dt){
    position = position + vitesse * dt;
}




