#ifndef AXE_H
#define AXE_H

#include <QOpenGLWidget>
#include <QOpenGLBuffer>
#include <QOpenGLShaderProgram>
#include <QOpenGLFunctions>

class Axe
{
public:
    Axe(QOpenGLWidget * parent);
    void set_size(float size);
    void make_axis_object();
    void init_axis();
    void draw_axis(QMatrix4x4 projection, QMatrix4x4 view);
    void destroy_vbo();

private:
    QVector<GLfloat> vert_data;
    QOpenGLBuffer vbo;
    QOpenGLShaderProgram *m_program;
    float size;

};

#endif // AXE_H
