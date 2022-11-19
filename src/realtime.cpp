#include "realtime.h"
#define GL_SILENCE_DEPRECATION

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

    // destroy all buffers
    Realtime::DestroyBuffers(true);

    // delete the camera is the scene is initialized
    if (Realtime::isInitialized) {
        delete Realtime::sceneCamera;
    }

    this->doneCurrent();
}

void Realtime::initializeGL() {

    std::cout << "initialize" << std::endl;

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
    Realtime::shader = ShaderLoader::createShaderProgram(":/resources/shaders/default.vert", ":/resources/shaders/default.frag");


//    Realtime::InitializeBuffers();
}

void Realtime::paintGL() {
//    std::cout << "paintgl top" << std::endl;
    // Students: anything requiring OpenGL calls every frame should be done here

    if (!Realtime::isInitialized) {
        return;
    }

    // if we have freshly loaded in a scene, initailize the buffers only once
    if (Realtime::changedScene) {
        // initilize all VBO and VAO data into the mesh objects
        Realtime::InitializeBuffers();
        Realtime::changedScene = false;
    }

//    std::cout << "paint gl loop" << std::endl;

    // Task 15: Clear the screen here
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // Bind the shader
    glUseProgram(Realtime::shader);

    // initialize uniforms, not per object
    Realtime::InitializeCameraUniforms();
    Realtime::InitializeLightUniforms();

    // initilialize uniforms per object, draw object
    Realtime::DrawBuffers();

    // Unbind the shader
    glUseProgram(0);

}

void Realtime::resizeGL(int w, int h) {

    std::cout << "resize" << std::endl;
    // Tells OpenGL how big the screen is
    glViewport(0, 0, size().width() * m_devicePixelRatio, size().height() * m_devicePixelRatio);

    // if the scene isn't initialized yet
    if (!Realtime::isInitialized) {
        return;
    }

    // Students: anything requiring OpenGL calls when the program starts should be done here
    // what does this mean?

    float aspectRatio = static_cast<float>(w) / static_cast<float>(h);

    if (Realtime::sceneCamera->getAspectRatio() != aspectRatio) {
        Realtime::sceneCamera->updateAspectRatio(aspectRatio);
    }

//    // destroy old buffers
//    Realtime::DestroyBuffers();
//    // reinitialize them
//    Realtime::InitializeBuffers();

}

void Realtime::sceneChanged() {
    // TODOs:
    std::cout << "scene changed" << std::endl;

    // destroy old meshes
    Realtime::DestroyMeshes();

    // destroy old meshes
    Realtime::DestroyBuffers(true);

    // delete the camera is the scene is initialized
    if (Realtime::isInitialized) {
        delete Realtime::sceneCamera;
    }

    // set current params (for on startup)
    Realtime::currentParam1 = settings.shapeParameter1;
    Realtime::currentParam2 = settings.shapeParameter2;


//    std::cout << "fuck" << std::endl;
//    std::cout << settings.farPlane << std::endl;
//    std::cout << settings.nearPlane << std::endl;


    // parse the scene that was stored in settings from the call to upload scenefile
    Realtime::sceneParser.parse(settings.sceneFilePath, Realtime::sceneRenderData);
    Camera* cam = new Camera(Realtime::sceneRenderData.cameraData, size().height(), size().width(), settings.farPlane, settings.nearPlane);
    Realtime::sceneCamera = cam;

    // build each primitive into a composite struct that contains the class for the trimesh, etc.
    // apply it to the realtime class
    Realtime::CompilePrimitiveMeshes();

    Realtime::isInitialized = true;
    Realtime::changedScene = true;

    update(); // asks for a PaintGL() call to occur
}

void Realtime::settingsChanged() {
//    std::cout << "sttings changed" << std::endl;

    // if the scene isn't initialized yet
    if (!Realtime::isInitialized) {
        return;
    }

    // near plane and far plane updates
    if (Realtime::sceneCamera->nearPlane != settings.nearPlane || Realtime::sceneCamera->farPlane != settings.farPlane) {

//        std::cout << "camera udpate" << std::endl;
        Realtime::sceneCamera->updateViewPlanes(settings.farPlane, settings.nearPlane);
    }

    // updates for tesselation params
    if (settings.shapeParameter1 != Realtime::currentParam1 || settings.shapeParameter2 != Realtime::currentParam2) {
        // set current params (for on startup)
        Realtime::currentParam1 = settings.shapeParameter1;
        Realtime::currentParam2 = settings.shapeParameter2;

//        std::cout << " tesselstaion udpate" << std::endl;

        // update tesselation parameters
        Realtime::UpdateTesselations();
        // destroy old buffers
        Realtime::DestroyBuffers(false);
        // destroy the old ones
        Realtime::InitializeBuffers();
    }


//    std::cout << "exit" << std::endl;

    // TODO: updates for extra credit features

//    Debug::glErrorCheck();
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
