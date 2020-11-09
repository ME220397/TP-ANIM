// Basé sur :
// CC-BY Edouard.Thiel@univ-amu.fr - 22/01/2019

#include "princ.h"
#include <QDebug>

Princ::Princ(QWidget *parent) : QMainWindow(parent)
{
    setupUi(this);
    connect(size_slider, SIGNAL(valueChanged(int)), glarea,  SLOT(on_axis_size_changed(int)));
    connect(xBox, SIGNAL(valueChanged(int)), glarea,  SLOT(on_v1_coeff_changed(int)));
    connect(yBox, SIGNAL(valueChanged(int)), glarea,  SLOT(on_v2_coeff_changed(int)));
    connect(zBox, SIGNAL(valueChanged(int)), glarea,  SLOT(on_v3_coeff_changed(int)));
    connect(tPoisson, SIGNAL(valueChanged(int)), glarea,  SLOT(on_taille_changed(int)));
    connect(aquaD, SIGNAL(valueChanged(int)), glarea,  SLOT(on_depth_changed(int)));
    connect(aquaH, SIGNAL(valueChanged(int)), glarea,  SLOT(on_height_changed(int)));
    connect(aquaW, SIGNAL(valueChanged(int)), glarea,  SLOT(on_width_changed(int)));
    connect(nbPoissons, SIGNAL(valueChanged(int)), glarea, SLOT(on_nb_poissons_changed(int)));
    connect(Start, SIGNAL(clicked()), glarea, SLOT(on_click_start()));

}
