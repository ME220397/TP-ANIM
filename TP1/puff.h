#ifndef PUFF_H
#define PUFF_H
#include <QVector3D>
#include <QTimer>
#include <QOpenGLShaderProgram>
#include <QOpenGLTexture>
#include <math.h>

class Puff
{

public:
    Puff(QVector3D position, float taille, QVector3D vecteur_vitesse, float temps_restant);
    void animate(float dt);
    void display();
    void set_particle(QOpenGLShaderProgram *program_particle);
    void set_texture(QOpenGLTexture *texture);
    void dec_alpha();
    float get_size();
    float get_remaining_time();
    void set_number_of_rows(int n);
    QVector2D get_offset();
    void set_inc_vit_text(float v);
    void set_color(QVector3D color);


protected slots:
    void onTimeout();
private:
    QVector3D position;
    float taille;
    QVector3D vecteur_vitesse;
    float temps_restant;
    float temps_max;
    float vitesse_change_text;
    QOpenGLShaderProgram *program_particule;
    QOpenGLTexture *texture;
    QVector4D color;
    float alpha;
    int texture_index;
    int number_of_rows;
    float inc_v_text;
};



#endif // PUFF_H
