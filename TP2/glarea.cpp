// Basé sur :
// CC-BY Edouard.Thiel@univ-amu.fr - 22/01/2019

#include "glarea.h"
#include <QDebug>
#include <QSurfaceFormat>
#include <QMatrix4x4>
#include <QDir>
#include <QStandardPaths>
#include <QCoreApplication>


// shader appliqué à chaque vertex
static const char *vertexShaderSource =
    "attribute vec4 in_position;               \n"
    "attribute lowp vec4 in_color;               \n"
    "varying lowp vec4 color;               \n"
    "uniform mat4 projectionMatrix;         \n"
    "uniform mat4 viewMatrix;               \n"
    "uniform mat4 modelMatrix;              \n"
    "uniform float particleSize;            \n"
    "void main() {                          \n"
    "   color = in_color;                        \n"
    "   vec4 pos = vec4(vec3(in_position) * particleSize, 1.0);                   \n"
    "   gl_Position = projectionMatrix * viewMatrix * modelMatrix * pos;          \n"
    "}                                      \n";

// shader appliqué à un fragment = un morceau rasterisé d'une primitive graphique
static const char *fragmentShaderSource =
    "varying lowp vec4 color;               \n"
    "void main() {                          \n"
    "   gl_FragColor = color;               \n"
    "}                                      \n";

GLArea::GLArea(QWidget *parent) :
    QOpenGLWidget(parent)
{
    QSurfaceFormat sf;
    sf.setDepthBufferSize(24);
    sf.setSamples(16);
    setFormat(sf);

    setEnabled(true);                   // événements clavier et souris
    setFocusPolicy(Qt::StrongFocus);    // accepte focus
    setFocus();                         // donne le focus

    timer = new QTimer(this);
    timer->setInterval(50);           // msec
    connect (timer, SIGNAL(timeout()), this, SLOT(onTimeout()));
    timer->start();
    elapsedTimer.start();

    axe = new Axe(this);
    aquarium = new Aquarium(this);
    poisson = new Poisson(QVector3D(0.0f, 0.0f, 0.0F), 1.f, QVector3D(0.0f, 0.0f, 1.0F));
    banc = new Banc(24, aquarium->get_height(), aquarium->get_width(), aquarium->get_depth());

}


GLArea::~GLArea()
{
    delete timer;

    // Contrairement aux méthodes virtuelles initializeGL, resizeGL et repaintGL,
    // dans le destructeur le contexte GL n'est pas automatiquement rendu courant.
    makeCurrent();
    tearGLObjects();
    doneCurrent();
}


void GLArea::initializeGL()
{
    initializeOpenGLFunctions();
    glClearColor(0.5f,0.5f,1.0f,1.0f);
    glEnable(GL_DEPTH_TEST);

    makeGLObjects();

    // shader du sol
    program_sol = new QOpenGLShaderProgram(this);
    program_sol->addShaderFromSourceFile(QOpenGLShader::Vertex, ":/shaders/simple.vsh");
    program_sol->addShaderFromSourceFile(QOpenGLShader::Fragment, ":/shaders/simple.fsh");
    if (! program_sol->link()) {  // édition de lien des shaders dans le shader program
        qWarning("Failed to compile and link shader program:");
        qWarning() << program_sol->log();
    }
    program_sol->setUniformValue("texture", 0);

    // shader de billboard
    program_particule = new QOpenGLShaderProgram(this);
    program_particule->addShaderFromSourceFile(QOpenGLShader::Vertex, ":/shaders/billboard.vsh");
    program_particule->addShaderFromSourceFile(QOpenGLShader::Fragment, ":/shaders/billboard.fsh");
    if (! program_particule->link()) {  // édition de lien des shaders dans le shader program
        qWarning("Failed to compile and link shader program:");
        qWarning() << program_particule->log();
    }
    program_particule->setUniformValue("texture", 0);

    //Shader du tetrahadre
    program_poisson = new QOpenGLShaderProgram(this);
    program_poisson->addShaderFromSourceCode(QOpenGLShader::Vertex, vertexShaderSource);
    program_poisson->addShaderFromSourceCode(QOpenGLShader::Fragment, fragmentShaderSource);
    if(!program_poisson->link()){
        qWarning("Failed to compile and link shader program");
        qWarning() << program_poisson->log();
    }

    // Shader des axes x, y et z
    axe->init_axis();
    // shader de l'aquarium
    aquarium->init_axis();

}


void GLArea::makeGLObjects()
{
    // Création du sol
    float tailleSol = 20.0f;

    GLfloat vertices_sol[] = {
       -tailleSol, 0.0f,-tailleSol,
       -tailleSol, 0.0f, tailleSol,
        tailleSol, 0.0f, tailleSol,
        tailleSol, 0.0f, tailleSol,
        tailleSol, 0.0f,-tailleSol,
       -tailleSol, 0.0f,-tailleSol
    };

    GLfloat texCoords_sol[] = {
            0.0f, 0.0f,
            0.0f, 1.0f,
            1.0f, 1.0f,
            1.0f, 1.0f,
            1.0f, 0.0f,
            0.0f, 0.0f
        };

    QVector<GLfloat> vertData_sol;
    for (int i = 0; i < 6; ++i) {
        // coordonnées sommets
        for (int j = 0; j < 3; j++)
            vertData_sol.append(vertices_sol[i*3+j]);
        // coordonnées texture
        for (int j = 0; j < 2; j++)
            vertData_sol.append(texCoords_sol[i*2+j]);
    }

    vbo_sol.create();
    vbo_sol.bind();
    vbo_sol.allocate(vertData_sol.constData(), vertData_sol.count() * int(sizeof(GLfloat)));


    // Création d'une particule de fumée
    GLfloat vertices_particule[] = {
       -0.5f, 0.5f, 0.0f,
       -0.5f,-0.5f, 0.0f,
        0.5f, 0.5f, 0.0f,
       -0.5f,-0.5f, 0.0f,
        0.5f,-0.5f, 0.0f,
        0.5f, 0.5f, 0.0f
    };

    GLfloat texCoords_particule[] = {
            0.0f, 0.0f,
            0.0f, 1.0f,
            1.0f, 0.0f,
            0.0f, 1.0f,
            1.0f, 1.0f,
            1.0f, 0.0f
        };

    QVector<GLfloat> vertData_particule;
    for (int i = 0; i < 6; ++i) {
        // coordonnées sommets
        for (int j = 0; j < 3; j++)
            vertData_particule.append(vertices_particule[i*3+j]);
        // coordonnées texture
        for (int j = 0; j < 2; j++)
            vertData_particule.append(texCoords_particule[i*2+j]);
    }

    vbo_particule.create();
    vbo_particule.bind();
    vbo_particule.allocate(vertData_particule.constData(), vertData_particule.count() * int(sizeof(GLfloat)));

    // Creation d'un tetrahede
    GLfloat vertices_poisson[] = {
        //triangle 1
        0.0, 0.5, 0.0,   // A
        -0.5, -0.5, 0.0, // B
        0.5, -0.5, 0.0,  // C

        //triangle 2
        0.0, -0.16, 1.5,   // D
        0.0, 0.5, 0.0,   // A
        0.5, -0.5,  0.0, // C

        //triangle 3
        -0.5, -0.5, 0.0, // B
        0.0, 0.5, 0.0,   // A
        0.0, -0.16, 1.5,   // D

        //triangle 4
        0.5, -0.5,  0.0, // C
        -0.5, -0.5, 0.0, // B
        0.0, -0.16, 1.5,   // D


    };

    GLfloat color_poisson[] = {

        // Sommets du triangle 1
        0.3, 0.3, 0.3,
        0.3, 0.3, 0.3,
        0.3, 0.3, 0.3,

        // Sommets du triangle 2
        0.3, 0.3, 0.3,
        0.3, 0.3, 0.3,
        0.3, 0.3, 0.3,

        // Sommets du triangle 3
        0.3, 0.3, 0.3,
        0.3, 0.3, 0.3,
        0.3, 0.3, 0.3,

        // Sommets du triangle 3
        0.3, 0.3, 0.3,
        0.3, 0.3, 0.3,
        0.3, 0.3, 0.3
    };

    QVector<GLfloat> vertData_poisson;
    for(int i=0 ; i < 9; i++){
        // Sommets du poisson
        for(int j=0; j<3; j++){
            vertData_poisson.append(vertices_poisson[i*3+j]);
        }
        // Couleur
        for(int j=0; j<3; j++){
            vertData_poisson.append(color_poisson[i*3+j]);
        }
    }
    vbo_poisson.create();
    vbo_poisson.bind();
    vbo_poisson.allocate(vertData_poisson.constData(), vertData_poisson.count() * int(sizeof (GLfloat)));

    // Creation des axes
    axe->make_axis_object();

    //Creation de l'aquarium
    aquarium->make_axis_object();

    // Création de textures
    QImage image_sol(":/textures/ground.jpg");
    if (image_sol.isNull())
        qDebug() << "load image ground.jpg failed";
    textures[0] = new QOpenGLTexture(image_sol);

    QImage image_particule(":/textures/puff.png");
    if (image_particule.isNull())
        qDebug() << "load image puff.png failed";
    textures[1] = new QOpenGLTexture(image_particule);
}


void GLArea::tearGLObjects()
{
    vbo_sol.destroy();
    vbo_particule.destroy();
    vbo_poisson.destroy();
    axe->destroy_vbo();
    aquarium->destroy_vbo();
    for (int i = 0; i < 2; i++)
        delete textures[i];
}


void GLArea::resizeGL(int w, int h)
{
    glViewport(0, 0, w, h);
    windowRatio = float(w) / h;
}


void GLArea::paintGL()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // Matrice de projection
    QMatrix4x4 projectionMatrix;
    projectionMatrix.perspective(45.0f, windowRatio, 1.0f, 1000.0f);

    // Matrice de vue (caméra)
    QMatrix4x4 viewMatrix;
    viewMatrix.translate(xPos, yPos, zPos);
    viewMatrix.rotate(xRot, 1, 0, 0);
    viewMatrix.rotate(yRot, 0, 1, 0);
    viewMatrix.rotate(zRot, 0, 0, 1);

    //Affichage du program poisson test
    axe->draw_axis(projectionMatrix, viewMatrix);

    //Creation de l'aquarium
    aquarium->draw_axis(projectionMatrix, viewMatrix);

    //Affichage des poissons
    vbo_poisson.bind();
    program_poisson->bind();
    /*poisson->set_program(program_poisson);
    poisson->anime(dt);
    poisson->affiche(projectionMatrix, viewMatrix);*/

    // Affichage de banc
    banc->set_projection_matrix(projectionMatrix);
    banc->set_view_matrix(viewMatrix);
    banc->set_program(program_poisson);
    banc->animate(dt);
    banc->affiche();

}



void GLArea::keyPressEvent(QKeyEvent *ev)
{
    float da = 1.0f;

    switch(ev->key()) {
        case Qt::Key_A :
            xRot -= da;
            update();
            break;

        case Qt::Key_Q :
            xRot += da;
            update();
            break;

        case Qt::Key_Z :
            yRot -= da;
            update();
            break;

        case Qt::Key_S :
            yRot += da;
            update();
            break;

        case Qt::Key_E :
            zRot -= da;
            update();
            break;

        case Qt::Key_D :
            zRot += da;
            update();
            break;
    }
}


void GLArea::keyReleaseEvent(QKeyEvent *ev)
{
    qDebug() << __FUNCTION__ << ev->text();
}


void GLArea::mousePressEvent(QMouseEvent *ev)
{
    lastPos = ev->pos();
}


void GLArea::mouseReleaseEvent(QMouseEvent *ev)
{
    qDebug() << __FUNCTION__ << ev->x() << ev->y() << ev->button();
}


void GLArea::mouseMoveEvent(QMouseEvent *ev)
{
    int dx = ev->x() - lastPos.x();
    int dy = ev->y() - lastPos.y();

    if (ev->buttons() & Qt::LeftButton) {
        xRot += dy;
        yRot += dx;
        update();
    } else if (ev->buttons() & Qt::RightButton) {
        xPos += dx/10.0f;
        yPos -= dy/10.0f;
        update();
    } else if (ev->buttons() & Qt::MidButton) {
        xPos += dx/10.0f;
        zPos += dy;
        update();
    }

    lastPos = ev->pos();
}


void GLArea::onTimeout()
{
    static qint64 old_chrono = elapsedTimer.elapsed(); // static : initialisation la première fois et conserve la dernière valeur
    qint64 chrono = elapsedTimer.elapsed();
    dt = (chrono - old_chrono) / 1000.0f;
    old_chrono = chrono;



    update();
}

void GLArea::on_axis_size_changed(int v){
    axe->set_size((float) v);
}

void GLArea::on_v1_coeff_changed(int v){
    banc->set_v1_coeff(v);
}

void GLArea::on_v2_coeff_changed(int v){
    banc->set_v2_coeff(v);
}

void GLArea::on_v3_coeff_changed(int v){
    banc->set_v3_coeff(v);
}

void GLArea::on_taille_changed(int v){
    poisson->set_size(v);
}

void GLArea::on_width_changed(int v){
    aquarium->set_width(v);
    banc->set_width_box(v);
}

void GLArea::on_depth_changed(int v){
    aquarium->set_depth(v);
    banc->set_depth_box(v);
}

void GLArea::on_height_changed(int v){
    aquarium->set_height(v);
    banc->set_height_box(v);
}

void GLArea::on_nb_poissons_changed(int v){
    int n = banc->get_size();
    if(n>v) {
        for(int i=0; i< n-v; i++){
            banc->remove_last();
        }
    }
    if(v>n){
        for(int i=0; i< v-n; i++){
            banc->add_poisson();
        }
    }
}

void GLArea::on_click_start(){
    banc->on_start();
}
