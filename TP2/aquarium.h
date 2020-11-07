#ifndef AQUARIUM_H
#define AQUARIUM_H

#include <QOpenGLWidget>
#include <QOpenGLBuffer>
#include <QOpenGLShaderProgram>
#include <QOpenGLFunctions>


class Aquarium
{
public:
    Aquarium(QOpenGLWidget * parent);
    void set_width(float w);
    void set_height(float h);
    void set_depth(float d);
    void make_axis_object();
    void init_axis();
    void draw_axis(QMatrix4x4 projection, QMatrix4x4 view);
    void destroy_vbo();

private:
    QVector<GLfloat> vert_data;
    QOpenGLBuffer vbo;
    QOpenGLShaderProgram *m_program;
    float height, width, depth;
};

#endif // AQUARIUM_H
