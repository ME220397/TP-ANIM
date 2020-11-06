#ifndef POISSON_H
#define POISSON_H
#include<QVector3D>
#include<QOpenGLShaderProgram>


class Poisson
{
public:
    // Constructor
    Poisson(QVector3D position, float taille, QVector3D vitesse);

    // Getters and Setters
    QVector3D get_position();
    float get_size();
    QVector3D get_speed();
    float get_dist_perimeter();

    void set_position(QVector3D position);
    void set_size(float size);
    void set_speed(QVector3D vitesse);
    void set_program(QOpenGLShaderProgram *program);

    // Functions
    void anime(float dt);
    bool dans_voisinage(QVector3D p);
    void affiche(QMatrix4x4 projectionMatrix, QMatrix4x4 viewMatrix);

private:
    QVector3D position;
    float taille;
    QVector3D vitesse;
    float dist_perimeter;
    QOpenGLShaderProgram * program_poisson;
};

#endif // POISSON_H
