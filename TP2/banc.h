#ifndef BANC_H
#define BANC_H

#include <QRandomGenerator>
#include "poisson.h"
#include <QVector3D>

#include <QOpenGLWidget>
#include <QOpenGLBuffer>
#include <QOpenGLShaderProgram>
#include <QOpenGLFunctions>

class Banc
{
public:
    Banc(int nb, int h, int w, int d);
    int get_size();
    void on_start();
    void set_v1_coeff(int v);
    void set_v2_coeff(int v);
    void set_v3_coeff(int v);
    void set_height_box(int h);
    void set_width_box(int w);
    void set_depth_box(int d);
    void set_projection_matrix(QMatrix4x4 p);
    void set_view_matrix(QMatrix4x4 v);
    void add_poisson();
    void remove_last();
    void init_banc();
    void affiche();
    void set_program(QOpenGLShaderProgram *p);
    void set_speed(QVector3D p);
    QVector3D get_speed();
    void animate(float dt);

private:
    int height_box;
    int width_box;
    int depth_box;

    bool start;
    QMatrix4x4 projection;
    QMatrix4x4 view;
    //Poisson
    int nb_poissons;
    float size_poisson;
    QVector3D speed;
    std::list<Poisson> poissons;
    float L;
    int v1_coeff, v2_coeff, v3_coeff;

};

#endif // BANC_H
