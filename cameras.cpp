#include "cameras.h"
#include <QDebug>

#include "camerasmemento.h"
#include "mainview.h"


std::shared_ptr<Cameras> Cameras::singletonInstance_ = nullptr;

std::shared_ptr<Cameras> Cameras::getInstance() {
    if (!singletonInstance_)
        singletonInstance_ = std::shared_ptr<Cameras>(new Cameras());

    return singletonInstance_;
}

Cameras::~Cameras(){
}

void Cameras::terminate() {
    camerasZBuffer_->clear();
    frustum_->clear();
    camera3DModel_->clear();
}

void Cameras::init(MainView *glMainView, float modelYMax) {
    maxCameraYPos_ = modelYMax;
    camerasZBuffer_ = std::unique_ptr<CamerasZBuffer>(new CamerasZBuffer(glMainView));
    cameraLabels_ = std::unique_ptr<CameraLabels>(new CameraLabels(glMainView));

    const glm::vec4 FRUSTUM_COLOR = glm::vec4(0.0f, 1.0f, 0.0f, 1.0f);
    frustum_ = std::unique_ptr<CameraViewFrustum>(new CameraViewFrustum(glMainView->context(),
                                                                                  maxCameraYPos_,
                                                                                  FRUSTUM_COLOR));
    camera3DModel_ = std::unique_ptr<Camera3DModel>(new Camera3DModel(glMainView->context()));
}

void  Cameras::addNewCamera(const glm::vec3 &pos) {
    if (cameras_.size() >= MAX_NUM_CAMERAS_)
        return;

    ++countCameras_;
    setMemento();

    selectedCamID_ = countCameras_;

    cameras_[selectedCamID_].camID = selectedCamID_;
    cameras_[selectedCamID_].position = glm::vec3(pos.x, maxCameraYPos_- (maxCameraYPos_/12), pos.z); //starting position
    cameras_[selectedCamID_].viewDirection = glm::vec3(-1.0f, -0.75f, 0.0f);  //default view direction
    cameras_[selectedCamID_].FOV = 60.0f;   //default FOV
    cameras_[selectedCamID_].aspectRatio = 1.33f;   //default aspect ratio
    cameras_[selectedCamID_].cameraName = "New Camera"; //default camera name

    camerasZBuffer_->generateCameraViewZBuffer(selectedCamID_);
    camerasZBuffer_->generateCameraView180FOVZBuffer(selectedCamID_);
    drawSelectedCameraZBuffer();

    cameraLabels_->addNewLabel(selectedCamID_);

    updateCameraView_ = true;
    emit cameraAdded(cameras_[selectedCamID_].camID, cameras_[selectedCamID_].cameraName);
    emit setSelectedCameraPropertiesInMainWindow(cameras_[selectedCamID_].FOV,
                                                 cameras_[selectedCamID_].aspectRatio,
                                                 cameras_[selectedCamID_].cameraName);
}

void Cameras::updateSelectedCamera(const glm::vec3 &pos, const glm::vec3 &viewDir) {
    cameras_[selectedCamID_].position = pos;
    cameras_[selectedCamID_].viewDirection = viewDir;
    drawSelectedCameraZBuffer();
}

void Cameras::drawCameraLabels(const glm::mat4 &V, const glm::mat4 &P) {
    QVector<glm::vec3> camPositions;

    for (const auto& camera : cameras_) {
        camPositions.push_back(camera.position);
    }

    cameraLabels_->drawCameraLabels(camPositions, V, P);
}

void Cameras::drawCameras(const glm::mat4 &MVP) {
    QVector<glm::mat4> camerasModelMatrix;

    for (const auto& camera : cameras_) {
        camerasModelMatrix.push_back(glm::inverse(getCameraViewMatrix(camera.camID)));
    }

    camera3DModel_->draw(camerasModelMatrix, MVP);
}

void Cameras::drawSelectedCameraFrustum(const glm::mat4& MVP) {
    frustum_->setFrustumProperties(getSelectedCameraFOV(), getSelectedCameraAspectRatio(),
                                   getSelectedCameraViewProjectorMatrix());
    frustum_->drawFrustum(MVP, getSelectedCameraViewZBuffer());
}

void Cameras::setSelectedCamera(int camID) {
    selectedCamID_ = camID;

    emit setSelectedCameraPropertiesInMainWindow(cameras_[selectedCamID_].FOV,
                                                 cameras_[selectedCamID_].aspectRatio,
                                                 cameras_[selectedCamID_].cameraName);
    updateCameraView_ = true;
}


void Cameras::setSelectedCameraFOV(int value) {
    if (!empty()) {
        cameras_[selectedCamID_].FOV = static_cast<float>(value);
        updateCameraView_ = true;
    }
}

void Cameras::setSelectedCameraAspectRatio(double value) {
    if (!empty()) {
        cameras_[selectedCamID_].aspectRatio = value;
        updateCameraView_ = true;
    }
}

void Cameras::setSelectedCameraName(QString camName) {
    if (!empty()) {
        cameras_[selectedCamID_].cameraName = camName;
        emit cameraNameModified(selectedCamID_ , camName);
    }
}

void Cameras::deleteSelectedCamera() {
    if (!empty()) {
        setMemento();

        cameras_.remove(selectedCamID_);
        deleteCameraResources(selectedCamID_);
   }
}

void Cameras::setMemento() {
    CamerasMemento memento;
    memento.setState(cameras_);
    mementoStack_.push(std::move(memento));
}

void Cameras::getMemento() {
    if (!mementoStack_.empty()) {

        QMap<int, Camera> newCameras = cameras_;
        cameras_ = std::move(mementoStack_.top().getState());   //store previous state in cameras

        if (newCameras.size() == cameras_.size()) {  //camera not added/deleted
            setSelectedCamera(selectedCamID_);
        }
        else if (newCameras.size() > cameras_.size()) { //delete camera to complete undo
            deleteCameraResources(getCamIDForUndoOperation(newCameras));
        }
        else {  //add camera to complete undo
            addCameraResources(getCamIDForUndoOperation(newCameras));
        }
        drawAllCameraZBuffers();
        mementoStack_.pop();
    }
}

Cameras::Cameras() :
    UP_(0.0f, 1.0f, 0.0f),
    nearZ(0.1f),
    farZ(150.0f),
    countCameras_(0),
    MAX_NUM_CAMERAS_(7),
    selectedCamID_(-1),
    updateCameraView_(false),
    camera180FOVViewOn_(false) { }


void Cameras::addCameraResources(const int camID) {
    selectedCamID_ = camID;

    camerasZBuffer_->generateCameraViewZBuffer(selectedCamID_);
    camerasZBuffer_->generateCameraView180FOVZBuffer(selectedCamID_);

    cameraLabels_->addNewLabel(selectedCamID_);

    emit cameraAdded(cameras_[selectedCamID_].camID, cameras_[selectedCamID_].cameraName);
    setSelectedCamera(selectedCamID_);
}


void Cameras::deleteCameraResources(const int camID) {
    cameraLabels_->deleteLabel(camID);
    camerasZBuffer_->deleteTexture(camID);
    emit cameraDeleted(camID);
    selectedCamID_ = cameras_.cbegin().key();

    if (cameras_.empty())
        emit setSelectedCameraPropertiesInMainWindow(0, 0, "");
    else
       setSelectedCamera(selectedCamID_);
}

void Cameras::drawSelectedCameraZBuffer() {
    camerasZBuffer_->drawCameraViewFrameBuffer(selectedCamID_,
                                               getSelectedCameraViewProjectorMatrix());

    if (camera180FOVViewOn_)
        camerasZBuffer_->drawCamera180FOVFrameBuffer(selectedCamID_,
                                                     getSelectedCamera180FOVProjectorMatrix());
}

void Cameras::drawAllCameraZBuffers() {
    for (const auto& camera : cameras_) {
        camerasZBuffer_->drawCameraViewFrameBuffer(camera.camID,
                                                   getCameraViewProjectorMatrix(camera.camID));
    }

    if (camera180FOVViewOn_)
        camerasZBuffer_->drawCamera180FOVFrameBuffer(selectedCamID_,
                                                     getSelectedCamera180FOVProjectorMatrix());
}

int Cameras::getCamIDForUndoOperation(QMap<int, Camera> &newState) {
    std::vector<int> v(1);

    auto vNewState  = newState.keys();
    auto vCurrentState = cameras_.keys();

    //finds the id of the camera that was added/deleted prior to the undo operation
    std::set_symmetric_difference(vNewState.begin(), vNewState.end(),
                                  vCurrentState.begin(), vCurrentState.end(),
                                  v.begin());

    return v[0];
}
