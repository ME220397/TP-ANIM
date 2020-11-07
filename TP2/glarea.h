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
#include <axe.h>
#include <poisson.h>
#include <aquarium.h>

class GLArea : public QOpenGLWidget,
               protected QOpenGLFunctions
{
    Q_OBJECT

public:
    explicit GLArea(QWidget *parent = nullptr);
    ~GLArea() override;

protected slots:
    void onTimeout();

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

private slots:
    void on_axis_size_changed(int v);
    void on_x_pos_changed(int v);
    void on_y_pos_changed(int v);
    void on_z_pos_changed(int v);
    void on_taille_changed(int v);
    void on_width_changed(int v);
    void on_depth_changed(int v);
    void on_height_changed(int v);
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
    QOpenGLBuffer vbo_sol;
    QOpenGLBuffer vbo_particule;
    QOpenGLTexture *textures[2];

    //Shader du poisson
    QOpenGLShaderProgram *program_poisson;
    QOpenGLBuffer vbo_poisson;

    // Creation des axes du repère
    Axe * axe;
    //Creation d'un aquarium
    Aquarium * aquarium;
    // Creaton du poisson
    Poisson * poisson;

    void makeGLObjects();
    void tearGLObjects();
};

#endif // GLAREA_H
