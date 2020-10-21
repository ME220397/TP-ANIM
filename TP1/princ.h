// Basé sur :
// CC-BY Edouard.Thiel@univ-amu.fr - 22/01/2019

#ifndef PRINC_H
#define PRINC_H

#include "ui_princ.h"
#include <QColorDialog>

class Princ : public QMainWindow, private Ui::Princ
{
    Q_OBJECT

public:
    explicit Princ(QWidget *parent = nullptr);
    int get_taille_slider();

private slots:
    void on_pushButton_clicked();

private:
    int taille_slider;
    bool dialVisible = false;
    QColorDialog *dial;
};

#endif // PRINC_H
