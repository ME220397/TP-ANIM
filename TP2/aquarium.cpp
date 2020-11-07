#include "aquarium.h"
static const char *vertexShaderSource =
    "attribute vec4 in_position;               \n"
    "attribute lowp vec4 col;               \n"
    "varying lowp vec4 color;               \n"
    "uniform mat4 projectionMatrix;         \n"
    "uniform mat4 viewMatrix;               \n"
    "uniform mat4 modelMatrix;              \n"
    "uniform float w;                       \n"
    "uniform float h;                       \n"
    "uniform float d;                       \n"
    "void main() {                          \n"
    "   color = col;                        \n"
    "   vec4 pos = vec4(vec3(in_position.x * w, in_position.y*h, in_position.z*d) , 1.0);                   \n"
    "   gl_Position = projectionMatrix * viewMatrix * modelMatrix * pos;          \n"
    "}                                      \n";

// shader appliqué à un fragment = un morceau rasterisé d'une primitive graphique
static const char *fragmentShaderSource =
    "varying lowp vec4 color;               \n"
    "void main() {                          \n"
    "   gl_FragColor = color;               \n"
    "}                                      \n";

Aquarium::Aquarium(QOpenGLWidget * parent)
{
    m_program = new QOpenGLShaderProgram(parent);
    width = 24.f;
    height = 14.f;
    depth = 23.f;
}

void Aquarium::set_width(float w){
    width = w;
}

void Aquarium::set_height(float h){
    height = h;
}

void Aquarium::set_depth(float d){
    depth = d;
}

void Aquarium::make_axis_object(){

    GLfloat vertices[] = {
        // face right
        // line 1
        1.f, 1.f, +1.F,
        1.f, 1.f, -1.f,

        // line 2
        1.f, 1.f, -1.F,
        1.f, -1.f, -1.f,

        // line 3
        1.f, -1.f, -1.f,
        1.f, -1.f, +1.f,

        // line 4
        1.f, -1.f, +1.f,
        1.f, 1.f, +1.F,

        //face front
        //line 5
        1.f, 1.f, -1.f,
        -1.f, 1.f, -1.f,

        //line 6
        -1.f, 1.f, -1.f,
        -1.f, -1.f, -1.f,

        //line 7
        -1.f, -1.f, -1.f,
        1.f, -1.f, -1.f,

        //face left
        //line 8
        -1.f, 1.f, -1.f,
        -1.f, 1.f, 1.f,

        //line 9
         -1.f, 1.f, 1.f,
         -1.f, -1.f, 1.f,

        //line 10
        -1.f, -1.f, 1.f,
        -1.f, -1.f, -1.f,

        // face back
        // line 11
        1.f, 1.f, 1.f,
        -1.f, 1.f, 1.f,

        // line 12
        1.f, -1.f, 1.f,
        -1.f, -1.f, 1.f,
    };

    GLfloat color[] = {
        // couleur line 1
        0.0, 0.0, 0.0,
        0.0, 0.0, 0.0,

        //couleur line 2
        0.0, 0.0, 0.0,
        0.0, 0.0, 0.0,

        //couleur line 3
        0.0, 0.0, 0.0,
        0.0, 0.0, 0.0,

        //couleur line 4
        0.0, 0.0, 0.0,
        0.0, 0.0, 0.0,

        //couleur line 5
        0.0, 0.0, 0.0,
        0.0, 0.0, 0.0,

        //couleur line 6
        0.0, 0.0, 0.0,
        0.0, 0.0, 0.0,

        //couleur line 7
        0.0, 0.0, 0.0,
        0.0, 0.0, 0.0,

        //couleur line 8
        0.0, 0.0, 0.0,
        0.0, 0.0, 0.0,

        //couleur line 9
        0.0, 0.0, 0.0,
        0.0, 0.0, 0.0,

        //couleur line 10
        0.0, 0.0, 0.0,
        0.0, 0.0, 0.0,

        //couleur line 11
        0.0, 0.0, 0.0,
        0.0, 0.0, 0.0,

        //couleur line 12
        0.0, 0.0, 0.0,
        0.0, 0.0, 0.0,

    };

    for(int i=0; i<24; i++){
        // sommets des Aquariums
        for(int j=0; j<3 ; j++)
            vert_data.append(vertices[i*3+j]);
        // Couleurs de Aquariums
        for(int j=0; j<3 ; j++)
            vert_data.append(color[i*3+j]);
    }

    vbo.create();
    vbo.bind();
    vbo.allocate(vert_data.constData(), vert_data.count() * sizeof (GLfloat));
}

void Aquarium::init_axis(){
    m_program->addShaderFromSourceCode(QOpenGLShader::Vertex, vertexShaderSource);
    m_program->addShaderFromSourceCode(QOpenGLShader::Fragment, fragmentShaderSource);
    if (! m_program->link()) {  // édition de lien des shaders dans le shader program
        qWarning("Failed to compile and link shader program:");
        qWarning() << m_program->log();
    }
}

void Aquarium::destroy_vbo(){
    vbo.destroy();
}

void Aquarium::draw_axis(QMatrix4x4 projection, QMatrix4x4 view){

    vbo.bind();
    m_program->bind();
    QMatrix4x4 modelAquariumMatrix;
    modelAquariumMatrix.translate(0.f, 0.f, 0.f);

    m_program->setUniformValue("projectionMatrix", projection);
    m_program->setUniformValue("viewMatrix", view);
    m_program->setUniformValue("modelMatrix", modelAquariumMatrix);

    m_program->setUniformValue("w", width);
    m_program->setUniformValue("h", height);
    m_program->setUniformValue("d", depth);
    m_program->setAttributeBuffer("in_position", GL_FLOAT ,0, 3,  6*sizeof(GLfloat));
    m_program->setAttributeBuffer("col", GL_FLOAT, 3*sizeof (GLfloat), 3, 6*sizeof(GLfloat));

    m_program->enableAttributeArray("in_position");
    m_program->enableAttributeArray("col");

    glDrawArrays(GL_LINES, 0, 24);

    m_program->disableAttributeArray("in_position");
    m_program->disableAttributeArray("col");

    m_program->release();

}
