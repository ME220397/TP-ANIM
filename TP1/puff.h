#ifndef PUFF_H
#define PUFF_H
#include <QVector3D>
#include <QTimer>
#include <QOpenGLShaderProgram>
#include <QOpenGLTexture>

class Puff
{

public:
    Puff(QVector3D position, float taille, QVector3D vecteur_vitesse, float temps_restant);
    void animate(float dt);
    void display();
    void set_particle(QOpenGLShaderProgram *program_particle);
    void set_texture(QOpenGLTexture *texture);

protected slots:
    void onTimeout();
private:
    QVector3D position;
    float taille;
    QVector3D vecteur_vitesse;
    float temps_restant;
    QOpenGLShaderProgram *program_particule;
    QOpenGLTexture *texture;
    //QTimer *m_time = nullptr;

};



#endif // PUFF_H
