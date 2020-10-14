#include "puff.h"

Puff::Puff(QVector3D position, float taille, QVector3D vecteur_vitesse, float temps_restant)
{
    //m_time = new QTimer(this);
    //connect(m_time, SIGNAL(timeout()), this, SLOT(onTimeout()));
    //m_time->setInterval(temps_restant*60);
    //m_time->start();
    this->position = position;
    this->taille = taille;
    this->vecteur_vitesse = vecteur_vitesse;
    this->temps_restant = temps_restant;
}

void Puff::animate(float dt){
    if(temps_restant > 0)
        position = position + vecteur_vitesse * dt;
        temps_restant -= dt;
}

void Puff::set_particle(QOpenGLShaderProgram *program_particle){
    this->program_particule = program_particle;
}

void Puff::set_texture(QOpenGLTexture *texture){
    this->texture = texture;
}

void Puff::display(){
    if(temps_restant > 0){
        QMatrix4x4 modelMatrixParticule;
        modelMatrixParticule.translate(position);
        program_particule->setUniformValue("modelMatrix", modelMatrixParticule);
        program_particule->setUniformValue("particleSize", taille); //taille de départ.

        program_particule->setAttributeBuffer("in_position", GL_FLOAT, 0, 3, 5 * sizeof(GLfloat));
        program_particule->setAttributeBuffer("in_uv", GL_FLOAT, 3 * sizeof(GLfloat), 2, 5 * sizeof(GLfloat));
        program_particule->enableAttributeArray("in_position");
        program_particule->enableAttributeArray("in_uv");

        texture->bind();
        glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);
        glEnable(GL_BLEND);
        glDrawArrays(GL_TRIANGLES, 0, 6);
        glDisable(GL_BLEND);
        texture->release();

        program_particule->disableAttributeArray("in_position");
        program_particule->disableAttributeArray("in_uv");
        program_particule->release();
    }
}
