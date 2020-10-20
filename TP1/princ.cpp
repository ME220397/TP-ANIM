// Basé sur :
// CC-BY Edouard.Thiel@univ-amu.fr - 22/01/2019

#include "princ.h"
#include <QDebug>

Princ::Princ(QWidget *parent) : QMainWindow(parent)
{
    setupUi(this);
}

int Princ::get_taille_slider(){
    return taille_slider;
}

void Princ::on_TailleSlider_sliderMoved(int position)
{
    taille_slider = position;
}
