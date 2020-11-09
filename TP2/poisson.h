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
    void set_dist_perimeter();

    // Functions
    void anime(float dt, int w, int h, int d);
    bool dans_voisinage(QVector3D p);
    void affiche(QMatrix4x4 projectionMatrix, QMatrix4x4 viewMatrix);
    float angle_v1_v2(QVector3D v1, QVector3D v2);


private:
    QVector3D position;
    float taille;
    QVector3D vitesse;
    QVector3D axe;
    float dist_perimeter;
    QOpenGLShaderProgram * program_poisson;
    int angle;
    int rot_x, rot_y, rot_z;
    QMatrix4x4 flocking_rotation;
    QVector3D rotation_axis;
    float time;
    bool speed_changed;
;    //Coefficient de maneauvrabilité.
    float L;
};

#endif // POISSON_H
