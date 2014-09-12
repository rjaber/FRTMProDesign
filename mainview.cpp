#include "mainview.h"

#include <QApplication>
#include <QDesktopWidget>
#include <QDebug>
#include <QWidget>
#include <QtCore>
#include <QKeyEvent>
#include <QMouseEvent>

#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_inverse.hpp>

#include "frtmutils.h"

MainView::MainView(const QGLFormat &format, const QString &mFilePath, QWidget *parent) :
         QGLWidget(format, parent),
         modelFilePath_(mFilePath),
         mainViewFOV_(60.0f) {

    FRTMUtils::setWidgetMinimumSize(this,
                                    0.5f,  //min width factor
                                    0.5f); //min height factor

    setFocusPolicy(Qt::ClickFocus);
    setMouseTracking(false);
    setAcceptDrops(true);

    cameras_ = Cameras::getInstance();
    view_ = std::unique_ptr<View>(new View(glm::vec3(0.0f, 30.0f, 0.0f),
                                           glm::vec3(0.0f, -1.0f, -1.0f),
                                           glm::vec3(0.0f, 1.0f, 0.0f),
                                           3.0f, 0.25f));
    pendingUpdate_ = true;
    cameraOn_ = false;
    changeScene_ = false;
    camera180FOVView_ = false; 
}


MainView::~MainView() {
    mainModel_.deleteModelResources();
    mainModel_.deleteVAOFromContext(this->context());
    cameras_->terminate();
    qDebug() <<"Shutdown Successfull";
}

void MainView::toggelShowCamera180FOVView(bool toggleVal) {
    camera180FOVView_ = toggleVal;
    cameras_->setCamera180FOVView(toggleVal);
}

void MainView::setMainViewWorldPosition(int camID) {
    view_->setGoToPosition(cameras_->getCameraPosition(camID));
    changeScene_ = true;
}

void MainView::initializeGL() {
    glewExperimental = GL_TRUE;

    GLenum err = glewInit();
    if (err != GLEW_OK) {
        qDebug() << "Error initializing GLEW";
        exit(EXIT_FAILURE);
    }

    //Initialize GL state
    glEnable(GL_MULTISAMPLE);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    mainModel_.buildCompleteShaderModel(modelFilePath_);
    shaderProgram_.buildShaderProgram();
    M_ = MainModel::M;

    shaderProgram_.use();
        const glm::vec4 Y_CLIP_PLANE = glm::vec4(0.0f, -1.0f, 0.0f, mainModel_.getModelYMax());
        glUniform4fv(shaderProgram_("yClipPlane"), 1, glm::value_ptr(Y_CLIP_PLANE));
        glUniform3fv(shaderProgram_("lightPosition"), 1, glm::value_ptr(MainModel::lightPosition));
        glUniformMatrix4fv(shaderProgram_("M"), 1, GL_FALSE, glm::value_ptr(M_));
    shaderProgram_.unUse();

    vaoMainViewID_ = mainModel_.getVAOForContext(shaderProgram_, this->context());
    cameras_->init(this, mainModel_.getModelYMax());

    qDebug() <<"MainView Initialization Successfull";
}

void MainView::paintGL() {

    glClearBufferfv(GL_COLOR, 0, glm::value_ptr(MainModel::skyColor));
    glClear(GL_DEPTH_BUFFER_BIT);
    glViewport(0,0,mainViewWidth_, mainViewHeight_);
    glEnable(GL_CULL_FACE);
    glEnable(GL_CLIP_PLANE0);

    cameraOn_ = !(cameras_->empty());

    V_ = view_->getViewMatrix();
    MV_ = V_ * M_;
    MVP_ = P_ * MV_;

    glBindVertexArray(vaoMainViewID_);
    shaderProgram_.use();

        static int numOfCams;
        numOfCams = cameras_->size();

        glUniform1i(shaderProgram_("numOfCams"), numOfCams);
        glUniformMatrix4fv(shaderProgram_("MV"), 1, GL_FALSE, glm::value_ptr(MV_));
        glUniformMatrix4fv(shaderProgram_("MVP"), 1, GL_FALSE, glm::value_ptr(MVP_));

        if (cameraOn_) {
            static glm::mat4 projectorMatrix[7] = {glm::mat4(0.0f)};


            //Loads all exisiting camera projector matrices and their corresponding
            //depth textures (z-buffers) for use in MainView's shader
            int cameraCount = 0;
            for (auto itrCamera = cameras_->cbegin(); itrCamera != cameras_->cend(); ++itrCamera) {
                projectorMatrix[cameraCount] = GLUtils::coordSpaceToTextureSamplingAdapter *
                                        cameras_->getCameraViewProjectorMatrix(itrCamera.key());

                glActiveTexture(GL_TEXTURE1 + cameraCount++);
                glBindTexture(GL_TEXTURE_2D, cameras_->getCameraViewZBuffer(itrCamera.key()));
            }

            glUniformMatrix4fv(shaderProgram_("projectionMat"), numOfCams, GL_FALSE, glm::value_ptr(projectorMatrix[0]));

            //If feature is enabled, loads 180 FOV camera projector matrix and its corresponding
            //depth texture (z-buffer) for use in MainView's shader
            if (camera180FOVView_) {
                static glm::mat4 projectorPotentialMatrix = glm::mat4(0.0f);
                projectorPotentialMatrix = GLUtils::coordSpaceToTextureSamplingAdapter *
                                            cameras_->getSelectedCamera180FOVProjectorMatrix();

                glUniform1i(shaderProgram_("is180FOVOn"), 1);
                glUniformMatrix4fv(shaderProgram_("potentialProjMat"), 1, GL_FALSE, glm::value_ptr(projectorPotentialMatrix));
                glActiveTexture(GL_TEXTURE8);
                glBindTexture(GL_TEXTURE_2D, cameras_->getSelectedCamera180FOVZBuffer());
            } else {
                glUniform1i(shaderProgram_("is180FOVOn"), 0);
            }
        }

        mainModel_.drawModel(shaderProgram_);

    shaderProgram_.unUse();

    if (cameraOn_) {
        cameras_->drawCameras(MVP_);
        cameras_->drawSelectedCameraFrustum(MVP_);
        cameras_->drawCameraLabels(V_, P_);
    }

    if (changeScene_ == true)
        changeScene_ = view_->gotoNewPosition();

    renderLater();
}

void MainView::resizeGL(int w, int h ) {
    mainViewWidth_ = w;
    mainViewHeight_ = h;
    glViewport(0, 0, mainViewWidth_, mainViewHeight_);
    
    static const float nearZ = 0.1f;
    static const float farZ = 300.0f;
    assert(mainViewHeight_ != 0);
    P_ = glm::perspective(mainViewFOV_, static_cast<float>(mainViewWidth_)/mainViewHeight_, nearZ, farZ);
}

void MainView::mousePressEvent(QMouseEvent *event) {
    view_->mouseDown(event->pos().x(), event->pos().y());
    if (event->buttons() == Qt::RightButton) {
        cameras_->setMemento(); //stores current state before moving camera's position
    }
}

void MainView::mouseMoveEvent(QMouseEvent *event) {
    //changes the position of selected camera by making it follow the mouse's cursor
    if (event->buttons() == Qt::RightButton) {
        cameras_->updateSelectedCameraPosition(getMouseCursorWolrdPosition(event->x(), event->y()));
    }
    //changes this GLWidget's view based on inputs from the mouse
    view_->mouseUpdate(event);
}

void MainView::dropEvent(QDropEvent *event) {
    cameras_->addNewCamera(getMouseCursorWolrdPosition(event->pos().x(), event->pos().y()));
    //view_->mouseDown(event->pos().x(), event->pos().y());
    event->acceptProposedAction();
}

void MainView::dragEnterEvent(QDragEnterEvent *event) {
    event->acceptProposedAction();
}

void MainView::dragMoveEvent(QDragMoveEvent *event) {
     Q_UNUSED(event);
}

void MainView::keyPressEvent(QKeyEvent *event) {
    switch(event->key()) {
        case Qt::Key_W:
            view_->moveFoward();
            break;
        case Qt::Key_S:
            view_->moveBackward();
            break;
        case Qt::Key_A:
            view_->strafeLeft();
            break;
        case Qt::Key_D:
            view_->strafeRight();
            break;
        case Qt::Key_Q:
            view_->moveUp();
            break;
        case Qt::Key_E:
            view_->moveDown();
            break;
        default:
            QGLWidget::keyPressEvent(event);
    }
}

bool MainView::event(QEvent *event) {
    //calls paintGL to enable rendering loop
    switch (event->type()) {
    case QEvent::UpdateRequest:
        pendingUpdate_ = false;
        updateGL();
        return true;
    default:
        return QGLWidget::event(event);
    }
}

glm::vec3 MainView::getMouseCursorWolrdPosition(float winPosX, float winPosY) {
    GLint viewport[4];
    glGetIntegerv(GL_VIEWPORT, viewport);

    //point 0 y-coord in opengl starts at the bottom, whereas in this window at the top
    //when calling glFunctions the y must be converted to suit opengl requirements
    float SCREEN_SPACE_Y = viewport[3] - winPosY;
    float SCREEN_SPACE_X = winPosX;

    GLfloat screenSpaceZ;
    glReadPixels(SCREEN_SPACE_X, SCREEN_SPACE_Y, 1, 1, GL_DEPTH_COMPONENT, GL_FLOAT, &screenSpaceZ);

    glm::vec3 screenSpacePos = glm::vec3(SCREEN_SPACE_X, SCREEN_SPACE_Y, screenSpaceZ);

    glm::vec3 worldSpacePos = glm::unProject(screenSpacePos, view_->getViewMatrix(),
                                   P_, glm::vec4(viewport[0], viewport[1], viewport[2], viewport[3]));

    return worldSpacePos;
}

void MainView::renderLater() {
    //implements rendering loop to enable continous calls to paintGL
    if (!pendingUpdate_) {
        pendingUpdate_ = true;
        QCoreApplication::postEvent(this, new QEvent(QEvent::UpdateRequest));
    }
}
