// Basé sur :
// CC-BY Edouard.Thiel@univ-amu.fr - 22/01/2019

#ifndef GLAREA_H
#define GLAREA_H

#include <QKeyEvent>
#include <QTimer>
#include <QElapsedTimer>
#include <QOpenGLWidget>
#include <QOpenGLFunctions>
#include <QOpenGLBuffer>
#include <QOpenGLShaderProgram>
#include <QOpenGLTexture>
#include "puff.h"
#include "smoke.h"

class GLArea : public QOpenGLWidget,
               protected QOpenGLFunctions
{
    Q_OBJECT

public:
    explicit GLArea(QWidget *parent = nullptr);
    ~GLArea() override;

protected slots:
    void onTimeout();
    void size_changed(int size);
    void vitesse_x_changed(int v);
    void vitesse_y_changed(int v);
    void vitesse_z_changed(int v);
    void vitesse_change_texture(int v);
    void color_puff_changed(QColor color);
    void puff_life_changed(int l);

protected:
    void initializeGL() override;
    void doProjection();
    void resizeGL(int w, int h) override;
    void paintGL() override;
    void keyPressEvent(QKeyEvent *ev) override;
    void keyReleaseEvent(QKeyEvent *ev) override;
    void mousePressEvent(QMouseEvent *ev) override;
    void mouseReleaseEvent(QMouseEvent *ev) override;
    void mouseMoveEvent(QMouseEvent *ev) override;

private:
    float xRot=20.0f, yRot=0.0f, zRot=0.0f;
    float xPos=0.0f,  yPos=0.0f, zPos=-50.0f;
    QTimer *timer = nullptr;
    QElapsedTimer elapsedTimer;
    float dt = 0;
    float windowRatio = 1.0f;
    QPoint lastPos;

    QOpenGLShaderProgram *program_sol;
    QOpenGLShaderProgram *program_particule;
    QOpenGLShaderProgram *program_particule2;
    QOpenGLBuffer vbo_sol;
    QOpenGLBuffer vbo_particule;
    QOpenGLTexture *textures[2];

    Puff *puff_test;
    Puff *puff_test2;
    Smoke *smoke;

    void makeGLObjects();
    void tearGLObjects();
};

#endif // GLAREA_H
