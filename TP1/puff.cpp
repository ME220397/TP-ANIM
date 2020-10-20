#include "puff.h"

Puff::Puff(QVector3D position, float taille, QVector3D vecteur_vitesse, float temps_restant)
{

    this->position = position;
    this->taille = taille;
    this->vecteur_vitesse = vecteur_vitesse;
    this->temps_restant = temps_restant;
    this->temps_max = temps_restant;
    this->vitesse_change_text = 0;
    this->alpha = 1.f;
    this->color = QVector4D(1.f,1.f,1.f, alpha);
    this->texture_index = 1;
}

QVector2D Puff::get_offset(){
    // get x offset
    int column = texture_index % number_of_rows;
    float x = (float) column / (float) number_of_rows;
    // get y offset
    int row = texture_index / number_of_rows;
    float y = (float) row / (float) number_of_rows;

    return QVector2D(x,y);
}

void Puff::set_number_of_rows(int n){
    number_of_rows = n;
}

float Puff::get_size(){
    return taille;
}

float Puff::get_remaining_time(){
    return temps_restant;
}

void Puff::animate(float dt){
    if(temps_restant > 0){
        position = position + vecteur_vitesse * dt;
        temps_restant -= dt;;
    }
}

void Puff::set_particle(QOpenGLShaderProgram *program_particle){
    this->program_particule = program_particle;
}

void Puff::set_texture(QOpenGLTexture *texture){
    this->texture = texture;
}

void Puff::dec_alpha(){
    alpha = temps_restant/5;
    if(alpha < 0)
        alpha = 0;
    color.setW(alpha);
}

void Puff::display(){
    if(temps_restant > 0){
        QMatrix4x4 modelMatrixParticule;
        //color.setAlpha(0.f);

        modelMatrixParticule.translate(position);

        vitesse_change_text += 0.4;
        taille+=0.1;
        texture_index = (int) floor(vitesse_change_text + temps_max- temps_restant)%15;
        texture->bind();
        glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);
        glEnable(GL_BLEND);
        program_particule->setUniformValue("modelMatrix", modelMatrixParticule);
        program_particule->setUniformValue("particleSize", taille); //taille de départ.
        program_particule->setUniformValue("color", color);
        program_particule->setUniformValue("number_of_rows",(float)number_of_rows);
        program_particule->setUniformValue("offset", get_offset());

        program_particule->setAttributeBuffer("in_position", GL_FLOAT, 0, 3, 5 * sizeof(GLfloat));
        program_particule->setAttributeBuffer("in_uv", GL_FLOAT, 3 * sizeof(GLfloat), 2, 5 * sizeof(GLfloat));
        program_particule->enableAttributeArray("in_position");
        program_particule->enableAttributeArray("in_uv");
        glDrawArrays(GL_TRIANGLES, 0, 6);
        glDisable(GL_BLEND);
        texture->release();

        program_particule->disableAttributeArray("in_position");
        program_particule->disableAttributeArray("in_uv");

    }
}
