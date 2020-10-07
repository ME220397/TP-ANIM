#include "puff.h"

Puff::Puff(QVector3D position, float taille, QVector3D vecteur_vitesse, float temps_restant)
{
    this->position = position;
    this->taille = taille;
    this->vecteur_vitesse = vecteur_vitesse;
    this->temps_restant = temps_restant;
}

void Puff::animate(float dt){
    position = position + vecteur_vitesse * dt;
    //soustraire temps ecoulé.
}

void Puff::set_particle(QOpenGLShaderProgram *program_particle){
    this->program_particule = program_particle;
}

void Puff::set_texture(QOpenGLTexture *texture){
    this->texture = texture;
}
void Puff::display(){
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
