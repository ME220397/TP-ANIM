#include "axe.h"

static const char *vertexShaderSource =
    "attribute vec4 in_position;               \n"
    "attribute lowp vec4 col;               \n"
    "varying lowp vec4 color;               \n"
    "uniform mat4 projectionMatrix;         \n"
    "uniform mat4 viewMatrix;               \n"
    "uniform mat4 modelMatrix;              \n"
    "uniform float particleSize;            \n"
    "void main() {                          \n"
    "   color = col;                        \n"
    "   vec4 pos = vec4(vec3(in_position) * particleSize, 1.0);                   \n"
    "   gl_Position = projectionMatrix * viewMatrix * modelMatrix * pos;          \n"
    "}                                      \n";

// shader appliqué à un fragment = un morceau rasterisé d'une primitive graphique
static const char *fragmentShaderSource =
    "varying lowp vec4 color;               \n"
    "void main() {                          \n"
    "   gl_FragColor = color;               \n"
    "}                                      \n";

Axe::Axe(QOpenGLWidget * parent)
{
    m_program = new QOpenGLShaderProgram(parent);
    size = 150.f;
}

void Axe::set_size(float size){
    this->size = size;
}

void Axe::make_axis_object(){

    GLfloat vertices[] = {
        // axe des x
        0.5, 0.0, 0.0,
        -0.5, 0.0, 0.0,

        // axe des y
        0.0, 0.5, 0.0,
        0.0, -0.5, 0.0,

        //axe des z
        0.0, 0.0, 0.5,
        0.0, 0.0, -0.5
    };

    GLfloat color[] = {
        // couleur axe des x
        0.0, 0.0, 1.0,
        0.0, 0.0, 1.0,

        //couleur axe des y
        1.0, 0.0, 0.0,
        1.0, 0.0, 0.0,

        //couleur axe des z
        0.0, 1.0, 0.0,
        0.0, 1.0, 0.0
    };

    for(int i=0; i<6; i++){
        // sommets des axes
        for(int j=0; j<3 ; j++)
            vert_data.append(vertices[i*3+j]);
        // Couleurs de axes
        for(int j=0; j<3 ; j++)
            vert_data.append(color[i*3+j]);
    }

    vbo.create();
    vbo.bind();
    vbo.allocate(vert_data.constData(), vert_data.count() * sizeof (GLfloat));
}

void Axe::init_axis(){
    m_program->addShaderFromSourceCode(QOpenGLShader::Vertex, vertexShaderSource);
    m_program->addShaderFromSourceCode(QOpenGLShader::Fragment, fragmentShaderSource);
    if (! m_program->link()) {  // édition de lien des shaders dans le shader program
        qWarning("Failed to compile and link shader program:");
        qWarning() << m_program->log();
    }
}

void Axe::destroy_vbo(){
    vbo.destroy();
}

void Axe::draw_axis(QMatrix4x4 projection, QMatrix4x4 view){

    vbo.bind();
    m_program->bind();
    QMatrix4x4 modelAxeMatrix;
    modelAxeMatrix.translate(0.f, 0.f, 0.f);

    m_program->setUniformValue("projectionMatrix", projection);
    m_program->setUniformValue("viewMatrix", view);
    m_program->setUniformValue("modelMatrix", modelAxeMatrix);

    m_program->setUniformValue("particleSize", size);
    m_program->setAttributeBuffer("in_position", GL_FLOAT ,0, 3,  6*sizeof(GLfloat));
    m_program->setAttributeBuffer("col", GL_FLOAT, 3*sizeof (GLfloat), 3, 6*sizeof(GLfloat));

    m_program->enableAttributeArray("in_position");
    m_program->enableAttributeArray("col");

    glDrawArrays(GL_LINES, 0, 6);

    m_program->disableAttributeArray("in_position");
    m_program->disableAttributeArray("col");

    m_program->release();

}
