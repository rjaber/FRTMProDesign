#include "cameras.h"

#include <QDebug>

#include "cameralabels.h"
#include "mainview.h"

Label::Label(int lblID, const MainView *glMainWidget) : LABEL_ID_(lblID)  {

       connect(this, SIGNAL(labelClicked(int)), Cameras::getInstance().get(), SLOT(setSelectedCamera(int)));
       connect(this, SIGNAL(goToCameraView(int)), glMainWidget, SLOT(setMainViewWorldPosition(int)));

       createGoToCameraAction();
}

void Label::goToCameraViewActionTriggered() {

    emit goToCameraView(LABEL_ID_);
}

void Label::mousePressEvent(QMouseEvent *event) {

    Q_UNUSED(event);
    emit labelClicked(LABEL_ID_);
}

void Label::createGoToCameraAction() {

    goToCameraViewAction_ = new QAction("Go To Camera View", this);
    goToCameraViewAction_->setIcon(QIcon(":/images/camera.png"));
    connect(goToCameraViewAction_, SIGNAL(triggered()), this, SLOT(goToCameraViewActionTriggered()));

    this->addAction(goToCameraViewAction_);
    this->setContextMenuPolicy(Qt::ActionsContextMenu);
}


//*********************************//
//Camera Labels Class Implementation

void CameraLabels::addNewLabel(int labelID) {
    cameraLabels_[labelID] = new Label(labelID, mainViewGLWidget_);
    cameraLabels_[labelID]->setParent(mainViewGLWidget_);
    cameraLabels_[labelID]->resize(LABEL_DIMENSION_, LABEL_DIMENSION_);
    cameraLabels_[labelID]->setWindowFlags(Qt::CustomizeWindowHint | Qt::FramelessWindowHint | Qt::SplashScreen);
    cameraLabels_[labelID]->setPixmap(QPixmap(":/images/camera_orange_background.png", 0, Qt::AutoColor));
    cameraLabels_[labelID]->setScaledContents(true);
    cameraLabels_[labelID]->show();
}

void CameraLabels::deleteLabel(int labelID){
    delete cameraLabels_[labelID];
    cameraLabels_.remove(labelID);
}

void CameraLabels::drawCameraLabels(const QVector<glm::vec3> &camPositions,
                                    const glm::mat4 &V, const glm::mat4 &P) {

    const QPoint NEW_WINDOW_POS = mainViewGLWidget_->mapToGlobal(mainViewGLWidget_->pos());
    static const QPoint DEFAULT_WINDOW_POS = NEW_WINDOW_POS;

    int camPositionsIndex=0;
    for (const auto& cameraLabel : cameraLabels_) {
        const glm::vec3 LABEL_WORLD_POS = camPositions[camPositionsIndex++];

        const glm::vec3 LABEL_SCREEN_POS = glm::project(glm::vec3(LABEL_WORLD_POS.x, LABEL_WORLD_POS.y,
                                                                  LABEL_WORLD_POS.z), V, P,
                                                        glm::vec4(0, 0, mainViewGLWidget_->width(),
                                                                  mainViewGLWidget_->height()));

        if (isLabelInsideWindow(LABEL_SCREEN_POS)) {
            const int DELTA_WINDOW_POS_X = -(DEFAULT_WINDOW_POS.x() - NEW_WINDOW_POS.x());
            const int DELTA_WINDOW_POS_Y = -(DEFAULT_WINDOW_POS.y() - NEW_WINDOW_POS.y());

            cameraLabel->setGeometry(QRect(static_cast<int>(LABEL_SCREEN_POS.x) +
                                                                    mainViewGLWidget_->pos().x() + DELTA_WINDOW_POS_X,
                                                                    static_cast<int>(mainViewGLWidget_->height() -
                                                                                     LABEL_SCREEN_POS.y) +
                                                                    DELTA_WINDOW_POS_Y,
                                                                    LABEL_DIMENSION_, LABEL_DIMENSION_));
            cameraLabel->show();
        } else
            cameraLabel->hide();
    }
}

bool CameraLabels::isLabelInsideWindow(const glm::vec3 &labelScreenPos) const {

    const int WINDOW_LEFT = mainViewGLWidget_->geometry().left();
    const int WINDOW_RIGHT = mainViewGLWidget_->geometry().right();
    const int WINDOW_TOP = mainViewGLWidget_->geometry().top();
    const int WINDOW_BOTTOM = mainViewGLWidget_->geometry().bottom();

    static const int WINDOW_MARGIN_Y = mainViewGLWidget_->mapToGlobal(mainViewGLWidget_->pos()).y();

    const int LABEL_POS_Y = labelScreenPos.y + WINDOW_MARGIN_Y;
    const int LABEL_POS_X = labelScreenPos.x + WINDOW_LEFT;

    return LABEL_POS_X > WINDOW_LEFT &&
           LABEL_POS_X < WINDOW_RIGHT &&
           LABEL_POS_Y > WINDOW_TOP &&
           LABEL_POS_Y < WINDOW_BOTTOM &&
           labelScreenPos.z < 1.0f; //false if label is behind the camera
}
