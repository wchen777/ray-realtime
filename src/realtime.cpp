#include "realtime.h"

#include <QCoreApplication>
#include <QMouseEvent>
#include <QKeyEvent>
#include <iostream>
#include "settings.h"
#include "utils/shaderloader.h"

// ================== Project 5: Lights, Camera

Realtime::Realtime(QWidget *parent)
    : QOpenGLWidget(parent)
{
    m_prev_mouse_pos = glm::vec2(size().width()/2, size().height()/2);
    setMouseTracking(true);
    setFocusPolicy(Qt::StrongFocus);

    m_keyMap[Qt::Key_W]       = false;
    m_keyMap[Qt::Key_A]       = false;
    m_keyMap[Qt::Key_S]       = false;
    m_keyMap[Qt::Key_D]       = false;
    m_keyMap[Qt::Key_Control] = false;
    m_keyMap[Qt::Key_Space]   = false;

    // If you must use this function, do not edit anything above this
}

void Realtime::finish() {
    killTimer(m_timer);
    this->makeCurrent();

    // Students: anything requiring OpenGL calls when the program exits should be done here

    glDeleteProgram(Realtime::shader);
    Realtime::DestroyMeshes();

    this->doneCurrent();
}

void Realtime::initializeGL() {
    m_devicePixelRatio = this->devicePixelRatio();

    m_timer = startTimer(1000/60);
    m_elapsedTimer.start();

    // Initializing GL.
    // GLEW (GL Extension Wrangler) provides access to OpenGL functions.
    glewExperimental = GL_TRUE;
    GLenum err = glewInit();
    if (err != GLEW_OK) {
        std::cerr << "Error while initializing GL: " << glewGetErrorString(err) << std::endl;
    }
    std::cout << "Initialized GL: Version " << glewGetString(GLEW_VERSION) << std::endl;

    // Allows OpenGL to draw objects appropriately on top of one another
    glEnable(GL_DEPTH_TEST);
    // Tells OpenGL to only draw the front face
    glEnable(GL_CULL_FACE);
    // Tells OpenGL how big the screen is
    glViewport(0, 0, size().width() * m_devicePixelRatio, size().height() * m_devicePixelRatio);

    // Students: anything requiring OpenGL calls when the program starts should be done here

    // initialize the shader
    Realtime::shader = ShaderLoader::createShaderProgram("resources/shaders/default.vert", "resources/shaders/default.frag");

    // bind object vbo's
    // create vao's

    // etc. etc.

    // unbind
}

void Realtime::paintGL() {
    // Students: anything requiring OpenGL calls every frame should be done here

}

void Realtime::resizeGL(int w, int h) {
    // Tells OpenGL how big the screen is
    glViewport(0, 0, size().width() * m_devicePixelRatio, size().height() * m_devicePixelRatio);

    // Students: anything requiring OpenGL calls when the program starts should be done here

    // TODO: not sure what exactly to do here
    // include the rebuild scene stuff here.
    // modify the camera's fields?

}

void Realtime::sceneChanged() {

    // TODOs:

    // destroy old meshes
    Realtime::DestroyMeshes();

    // set current params (for on startup)
    Realtime::currentParam1 = settings.shapeParameter1;
    Realtime::currentParam2 = settings.shapeParameter2;

    // parse the scene that was stored in settings from the call to upload scenefile
    Realtime::sceneParser.parse(settings.sceneFilePath, Realtime::sceneRenderData);
    Camera cam(Realtime::sceneRenderData.cameraData, size().height(), size().width(), settings.farPlane, settings.nearPlane);
    Realtime::sceneCamera = cam;

    // build each primitive into a composite struct that contains the class for the trimesh, etc.
    // apply it to the realtime class
    Realtime::CompilePrimitiveMeshes();

    update(); // asks for a PaintGL() call to occur
}

void Realtime::settingsChanged() {

    // near plane and far plane updates
    if (Realtime::sceneCamera.nearPlane != settings.nearPlane || Realtime::sceneCamera.farPlane != settings.farPlane) {
        Realtime::sceneCamera.updateViewPlanes(settings.farPlane, settings.nearPlane);
    }

    // updates for tesselation params
    if (settings.shapeParameter1 != Realtime::currentParam1 || settings.shapeParameter2 != Realtime::currentParam2) {
        // set current params (for on startup)
        Realtime::currentParam1 = settings.shapeParameter1;
        Realtime::currentParam2 = settings.shapeParameter2;

        Realtime::UpdateTesselations();
    }

    // TODO: updates for extra credit features


    update(); // asks for a PaintGL() call to occur
}

// ================== Project 6: Action!

void Realtime::keyPressEvent(QKeyEvent *event) {
    m_keyMap[Qt::Key(event->key())] = true;
}

void Realtime::keyReleaseEvent(QKeyEvent *event) {
    m_keyMap[Qt::Key(event->key())] = false;
}

void Realtime::mousePressEvent(QMouseEvent *event) {
    if (event->buttons().testFlag(Qt::LeftButton)) {
        m_mouseDown = true;
        m_prev_mouse_pos = glm::vec2(event->position().x(), event->position().y());
    }
}

void Realtime::mouseReleaseEvent(QMouseEvent *event) {
    if (!event->buttons().testFlag(Qt::LeftButton)) {
        m_mouseDown = false;
    }
}

void Realtime::mouseMoveEvent(QMouseEvent *event) {
    if (m_mouseDown) {
        int posX = event->position().x();
        int posY = event->position().y();
        int deltaX = posX - m_prev_mouse_pos.x;
        int deltaY = posY - m_prev_mouse_pos.y;
        m_prev_mouse_pos = glm::vec2(posX, posY);

        // Use deltaX and deltaY here to rotate

        update(); // asks for a PaintGL() call to occur
    }
}

void Realtime::timerEvent(QTimerEvent *event) {
    int elapsedms   = m_elapsedTimer.elapsed();
    float deltaTime = elapsedms * 0.001f;
    m_elapsedTimer.restart();

    // Use deltaTime and m_keyMap here to move around

    update(); // asks for a PaintGL() call to occur
}
