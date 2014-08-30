#include "cameraview.h"

#include <memory>
#include <QtCore>
#include <QApplication>
#include <QDesktopWidget>
#include <QOpenGLContext>

#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_inverse.hpp>
#include <opencv2/highgui/highgui.hpp>

#include "mainview.h"
#include "view.h"
#include "glslcameraviewprogram.h"
#include "frtmutils.h"


CameraView::CameraView(MainView *mainViewGLWidget, QWidget *parent) :
    QGLWidget( mainViewGLWidget->format(), parent, mainViewGLWidget ),
    model_( mainViewGLWidget->getModel() ) {

    FRTMUtils::setWidgetMinimumSize(this,
                                    0.2f,   //min width factor
                                    0.35f); //min height factor

    setFocusPolicy(Qt::ClickFocus);
    setMouseTracking(false);

    cameras_ = Cameras::getInstance();
    view_ = std::unique_ptr<View>(new View(glm::vec3(0.0f, 0.0f, 0.0f),
                                           glm::vec3(0.0f, 0.0f, 0.0f),
                                           glm::vec3(0.0f, 1.0f, 0.0f),
                                           0.0f, 0.25f));
    pendingUpdate_ = true;
}

CameraView::~CameraView() {
    model_.deleteVAOFromContext(this->context());
}

void CameraView::initializeGL() {
    glewExperimental = GL_TRUE;

    GLenum err = glewInit();
    if (err != GLEW_OK) {
        qDebug() << "Error initializing GLEW";
        exit(EXIT_FAILURE);
    }

    //Initialize GL
    glEnable(GL_SCISSOR_TEST);
    glEnable(GL_CULL_FACE);
    glEnable(GL_DEPTH_TEST);

    M_ = MainModel::M;
    shaderProgram_.buildShaderProgram();
    shaderProgram_.use();
        glUniformMatrix4fv(shaderProgram_("M"), 1, GL_FALSE, glm::value_ptr(M_));
        glUniform3fv(shaderProgram_("lightPosition"), 1, glm::value_ptr(MainModel::lightPosition));
    shaderProgram_.unUse();

    vaoID_ = model_.getVAOForContext(shaderProgram_, this->context());
}

void CameraView::paintGL() {
    if (!cameras_->empty()) {

        if (cameras_->isModified()){
            updateView();
        }

        glClearBufferfv(GL_COLOR, 0, glm::value_ptr(MainModel::skyColor));
        glClear(GL_DEPTH_BUFFER_BIT);

        V_ = view_->getViewMatrix();
        MV_ = V_ * M_;
        MVP_ = P_ * MV_;

        glBindVertexArray(vaoID_);
        shaderProgram_.use();
            glUniformMatrix4fv(shaderProgram_("MV"), 1, GL_FALSE, glm::value_ptr(MV_));
            glUniformMatrix4fv(shaderProgram_("MVP"), 1, GL_FALSE, glm::value_ptr(MVP_));

            model_.drawModel(shaderProgram_);
        shaderProgram_.unUse();

        cameras_->updateSelectedCamera(view_->getPosition(), view_->getViewDirection());
    } else {
        //grey screen to signify no selected camera
        glScissor(0, 0, this->width(), this->height());
        static const glm::vec4 cameraOffColor = glm::vec4(0.5f, 0.5f, 0.5f, 1.0f);
        glClearBufferfv(GL_COLOR, 0, glm::value_ptr(cameraOffColor));
    }

    renderLater();
}

void CameraView::resizeGL(int w, int h ) {
    if (!cameras_->empty())
        updatePerspectiveView();    //update the viewport based on the FOV and aspect ratio of selected camera
    else
        glViewport(0, 0, w, h);
}

bool CameraView::event(QEvent *event) {
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

void CameraView::mousePressEvent(QMouseEvent *event) {
    cameras_->setMemento(); //stores current view direction for future undo operations (if any)
    view_->mouseDown(event->x(), event->y());
}

void CameraView::mouseMoveEvent(QMouseEvent *event) {
    view_->mouseUpdate(event);
}

void CameraView::renderLater() {
    //implements rendering loop to enable continous calls to paintGL
    if (!pendingUpdate_) {
        pendingUpdate_ = true;
        QCoreApplication::postEvent(this, new QEvent(QEvent::UpdateRequest));
    }
}

void CameraView::updateView() {
    view_->setView(cameras_->getSelectedCameraPosition(), cameras_->getSelectedCameraViewDirection());
    updatePerspectiveView();
}

void CameraView::updatePerspectiveView() {
    clearScreenBlack();

    const float CAMERA_AP = cameras_->getSelectedCameraAspectRatio();

    //calculates viewport based on camera's aspect ratio and FOV.
    //dynamically adjusts the viewport based on this GLWidget's dimensions
    //to draw all what the camera captures.
    int cameraViewWidth = this->width();
    int cameraViewHeight = cameraViewWidth * 1/CAMERA_AP;

    if (cameraViewHeight > this->height()) {
        cameraViewHeight = this->height();
        cameraViewWidth = cameraViewHeight * CAMERA_AP;
    }

    const int CAMERAVIEW_LEFT_SPACING = (this->width() - cameraViewWidth)/2;
    const int CAMERAVIEW_HEIGHT_SPACING = (this->height() - cameraViewHeight)/2;

    glViewport(CAMERAVIEW_LEFT_SPACING, CAMERAVIEW_HEIGHT_SPACING, cameraViewWidth, cameraViewHeight);
    glScissor(CAMERAVIEW_LEFT_SPACING, CAMERAVIEW_HEIGHT_SPACING, cameraViewWidth, cameraViewHeight);

    P_ = cameras_->getSelectedCameraPerspectiveMatrix();
}

void CameraView::clearScreenBlack() {
    glViewport(0, 0, this->width(), this->height());
    glScissor(0, 0, this->width(), this->height());

    static const glm::vec4 blackColor  = glm::vec4(0.0f);
    glClearBufferfv(GL_COLOR, 0, glm::value_ptr(blackColor));

    //    this->makeCurrent();
    //    this->swapBuffers();
}
