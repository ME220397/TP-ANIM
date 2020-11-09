#include "banc.h"
#include <QVector3D>
Banc::Banc(int nb, int h, int w, int d)
{
    start = false;
    nb_poissons = nb;
    width_box = w;
    height_box = h;
    depth_box = d;
    size_poisson = 0.5f;
    speed = QVector3D(0.f, 0.f, 1.f);
    init_banc();
    L = 0.1;
    v1_coeff = 1;
    v2_coeff = 1;
    v3_coeff = 1;

}
void Banc::add_poisson(){
    int x, y, z;
    int max_x = width_box - 1;
    int max_y = height_box - 1;
    int max_z = depth_box - 1;
    int max_speed = 6;
    int min_speed = -4;
    int speed_x, speed_y, speed_z;

    x = QRandomGenerator::global()->bounded(-max_x, max_x);
    y = QRandomGenerator::global()->bounded(-max_y, max_y);
    z = QRandomGenerator::global()->bounded(-max_z, max_z);
    speed_x = QRandomGenerator::global()->bounded(min_speed, max_speed);
    speed_y = QRandomGenerator::global()->bounded(min_speed, max_speed);
    speed_z = QRandomGenerator::global()->bounded(min_speed, max_speed);

    poissons.push_back(Poisson(QVector3D(x,y,z), size_poisson, QVector3D(speed_x, speed_y, speed_z)));
    nb_poissons++;
}

int Banc::get_size(){
    return nb_poissons;
}

QVector3D Banc::get_speed(){
    return speed;
}
void Banc::set_speed(QVector3D p){
    speed = p;
    std::list<Poisson>::iterator i;
    i = poissons.begin();
    while(i!=poissons.end()){
        i->set_speed(speed);
        i++;
    }
}
void Banc::on_start(){
    start = true;
}
void Banc::remove_last(){
    poissons.pop_back();
    nb_poissons--;
}
void Banc::init_banc(){
    int n = nb_poissons;
    for(int i=0; i<n; i++){
        add_poisson();
    }
    nb_poissons = n;
}

void Banc::set_program(QOpenGLShaderProgram *p){
    std::list<Poisson>::iterator i;
    i = poissons.begin();
    while(i!=poissons.end()){
        i->set_program(p);
        i++;
    }
}

void Banc::set_v1_coeff(int v){
    v1_coeff = v;
}

void Banc::set_v2_coeff(int v){
    v2_coeff = v;
}
void Banc::set_v3_coeff(int v){
    v3_coeff = v;
}

void Banc::set_depth_box(int d){
    depth_box = d;
}

void Banc::set_width_box(int w){
    width_box = w;
}

void Banc::set_height_box(int h){
    height_box = h;
}
void Banc::set_projection_matrix(QMatrix4x4 p){
    projection = p;
}

void Banc::set_view_matrix(QMatrix4x4 v){
    view = v;
}

void Banc::animate(float dt){

    if(start){
        QVector3D v1;
        QVector3D v2;
        QVector3D v3;
        std::list<Poisson>::iterator i;
        std::list<Poisson>::iterator j;
        int n_voisin;
        i = poissons.begin();
        while(i != poissons.end()){
            v1 = QVector3D(0, 0, 0);
            v2 = QVector3D(0 ,0, 0);
            v3 = QVector3D(0 ,0, 0);
            n_voisin = 0;
            j = poissons.begin();
            while(j != poissons.end()){
                if( j != i){
                    if(i->dans_voisinage(j->get_position())){
                        n_voisin++;
                        // Colision Avoidance
                        v1 += j->get_position() - i->get_position();
                        //Alignement
                        v2 += j->get_speed();
                        //Cohésion
                        v3 += j->get_position();
                    }
                }
                j++;
            }
            if(n_voisin > 0){
                v1 = v1/n_voisin;
                v1*=-1;
                v1.normalize();
                v2 = v2/n_voisin;
                v2.normalize();
                v3 = v3/n_voisin;
                v3 = v3 - i->get_position();
                v3.normalize();
                QVector3D v = v1_coeff * v1 + v2_coeff *v2 + v3_coeff * v3;
                i->set_speed(v);
            }
            i++;
        }
        //
        i = poissons.begin();
        while(i != poissons.end()){
            i->anime(dt, width_box, height_box, depth_box);
            i++;
        }
    }
}

void Banc::affiche(){
    std::list<Poisson>::iterator i;
    i = poissons.begin();
    while(i!=poissons.end()){
        i->affiche(projection, view);
        i++;
    }
}
