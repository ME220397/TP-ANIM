#ifndef BANC_H
#define BANC_H
#include "poisson.h"

class Banc
{
public:
    Banc();
    void set_height_box();


private:
    int height_box;
    int lenght_box;
    int depth_box;
    //Poisson
    int nb_poissons;
    QList<Poisson> poissons;

};

#endif // BANC_H
