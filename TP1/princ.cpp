// Basé sur :
// CC-BY Edouard.Thiel@univ-amu.fr - 22/01/2019

#include "princ.h"
#include <QDebug>

Princ::Princ(QWidget *parent) : QMainWindow(parent)
{
    setupUi(this);
    dial = new QColorDialog(this);
    dial->setCurrentColor(QColor(150,150,150));
    connect(TailleSlider, SIGNAL(sliderMoved(int)), glarea, SLOT(size_changed(int)));
    connect(VitesseX, SIGNAL(valueChanged(int)), glarea, SLOT(vitesse_x_changed(int)));
    connect(VitesseY, SIGNAL(valueChanged(int)), glarea, SLOT(vitesse_y_changed(int)));
    connect(VitesseZ, SIGNAL(valueChanged(int)), glarea, SLOT(vitesse_z_changed(int)));
    connect(VitText, SIGNAL(sliderMoved(int)), glarea, SLOT(vitesse_change_texture(int)));
    connect(dial, SIGNAL(currentColorChanged(QColor)), glarea, SLOT(color_puff_changed(QColor)));
}

int Princ::get_taille_slider(){
    return taille_slider;
}

void Princ::on_pushButton_clicked()
{
    if(dialVisible){
        dial->hide();
        dialVisible = false;
    }
    else{
        dial->show();
        dialVisible = true;
    }
}
