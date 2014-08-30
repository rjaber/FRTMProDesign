#ifndef CAMERAS_H
#define CAMERAS_H
#include <memory>
#include <iterator>
#include <QVector>
#include <QStack>
#include <QMap>
#include <QLabel>

#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>

#include "model.h"
#include "cameraviewfrustum.h"
#include "cameraszbuffer.h"
#include "camera3dmodel.h"
#include "cameralabels.h"

class Label;
class MainView;
class CamerasMemento;

/**
 * @brief Struct consisting of internal data of each Camera added in the MainView QGLWidget
 */
struct Camera {
    int camID;
    QString cameraName;
    glm::vec3 position;
    glm::vec3 viewDirection;
    float FOV;
    float aspectRatio;
};

/**
 *@brief Camera Container Manager
 *
 * Provides a container wrapper to store Camera(s) added in the MainView QGLWidget.
 * Adds extra features in order to manage Camera(s) in the MainView QGLWidget.
 *
 * This class is used to:
 *
 * -# Add and delete cameras.
 * -# Update Camera(s) data.
 * -# Query Camera(s) data and Z-Buffer (Depth) textures.
 * -# Perform undo operations.
 * -# Draw camera Label(s), 3D Model(s), and the selected CameraViewFrustum.
 *
 * This class is a singleton and aims to serve as a mediator between the
 * MainView and CameraView QGLWidgets.
 *
 * The singleton object is initialized from within MainView as follows:
 * @code
 * std::shared_ptr<Cameras> cameras = Cameras::getInstance();
 * cameras->init(this, modelYMax);  //modelYMax is the maximum Y-coordinate point in the loaded .obj model
 * @endcode
 */

class Cameras final : public QObject {
      Q_OBJECT
public:
    /**
     * @brief Gets unique instance
     * @return shared_ptr to the singleton instance
     */
    static std::shared_ptr<Cameras> getInstance();

    /** @brief Copying is not allowed*/
    Cameras(const Cameras&) = delete;

    /** @brief Copying is not allowed*/
    Cameras& operator=(const Cameras&) = delete;

    /** @brief Destructor */
    ~Cameras();

    /**
     * @brief init Initializes Cameras for use
     * @param glMainView MainView GLWidget.
     * @param Maximum Y-coordinate point in the loaded .obj model.
     */
    void init(MainView *glMainView, float modelYMax);

    /**
     * @brief Releases resources and restarts object's state
     */
    void terminate();

    /**
     * @brief Adds new Camera
     * @param pos World position of the Camera to be added.
     */
    void addNewCamera(const glm::vec3 &pos);

    /**
     * @brief Updates the selected Camera's position
     * @param pos New world position of selected Camera.
     */
    void updateSelectedCameraPosition(const glm::vec3 &pos) {
        cameras_[selectedCamID_].position.x = pos.x;
        cameras_[selectedCamID_].position.z = pos.z;
        updateCameraView_ = true;
    }

    /**
     * @brief Updates selected Camera's position and view direction
     * @param pos New world position of selected Camera.
     * @param viewDir New view direction of selected Camera.
     */
    void updateSelectedCamera(const glm::vec3 &pos, const glm::vec3 &viewDir);

    /**
     * @brief Draws current Camera Label(s)
     * @param V View matrix (i.e. lookAt matrix) of MainView.
     * @param P Perspective projection matrix of MainView.
     */
    void drawCameraLabels(const glm::mat4 &V, const glm::mat4 &P);

    /**
     * @brief Draws current Camera 3D Model(s)
     * @param MVP Model View Projection matrix of MainView.
     */
    void drawCameras(const glm::mat4& MVP);

    /**
     * @brief Draws the selected Camera's frustum
     * @@param MVP Model View Projection matrix of MainView.
     */
    void drawSelectedCameraFrustum(const glm::mat4 &MVP);

    /**
     * @return Whether Cameras' state has been modified
     */
    bool isModified(){
        if (updateCameraView_){
            updateCameraView_ = false;
            return true;
        } else {
            return false;
        }
    }

    /**
     * @brief Enables/Disables Camera 180 FOV Full Coverage Feature
     * @param toggleVal True to enbale Camera 180 FOV Full Coverage Feature, false otherwise.
     */
    void setCamera180FOVView(bool toggleVal) {
       camera180FOVViewOn_ = toggleVal;
    }

    /**
     * @brief Sets selected Camera's aspect ratio
     * @param ar New aspect ratio.
     */
    void setCamerasAspectRatio(const float ar) {
        camViewAspectRatio_ = ar;
    }

    /**
     * @return Selected Camera FOV
     */
    int getSelectedCameraFOV() const {
        return static_cast<int>(cameras_[selectedCamID_].FOV);
    }

    /**
     * @return Selected Camera Aspect Ratio
     */
    float getSelectedCameraAspectRatio() const {
        return cameras_[selectedCamID_].aspectRatio;
    }

    /**
     * @return Selected Camera Perspective Projection Matrix
     */
    glm::mat4 getSelectedCameraPerspectiveMatrix() const {
        return glm::perspective(static_cast<float>(getSelectedCameraFOV()),
                                getSelectedCameraAspectRatio(), nearZ, farZ);
    }

    /**
     * @return Selected Camera Z-Buffer(Depth) Texture ID
     */
    GLuint getSelectedCameraViewZBuffer() const {
        return camerasZBuffer_->getCameraViewZBuffer(selectedCamID_);
    }

    /**
     * @param camID Camera ID
     * @return Camera Z-Buffer(Depth) Texture ID
     */
    GLuint getCameraViewZBuffer(const int camID) const {
        return camerasZBuffer_->getCameraViewZBuffer(camID);
    }

    /**
     * @return Selected Camera Z-Buffer(Depth) Texture ID for 180 FOV Full Coverage Feature
     */
    GLuint getSelectedCamera180FOVZBuffer() const {
        return camerasZBuffer_->getCamera180FOVZBuffer(selectedCamID_);
    }

    /**
     * @return Selected Camera View Matrix (i.e. lookAt Matrix)
     */
    glm::mat4 getSelectedCameraViewMatrix() const {
        return glm::lookAt(cameras_[selectedCamID_].position,
                           cameras_[selectedCamID_].position + cameras_[selectedCamID_].viewDirection,
                           UP_);
    }

    /**
     * @param camID Camera ID
     * @return View Matrix (i.e. lookAt Matrix) of Camera camID
     */
    glm::mat4 getCameraViewMatrix(const int camID) const {
        return glm::lookAt(cameras_[camID].position,
                           cameras_[camID].position + cameras_[camID].viewDirection,
                           UP_);
    }

    /**
     * @return Selected Camera world position
     */
    glm::vec3 getSelectedCameraPosition() const {
        return cameras_[selectedCamID_].position;
    }

    /**
     * @param camID Camera ID
     * @return World position of Camera camID
     */
    glm::vec3 getCameraPosition(const int camID) const {
        return cameras_[camID].position;
    }

    /**
     * @return Selected Camera view direction
     */
    glm::vec3 getSelectedCameraViewDirection() const {
        return cameras_[selectedCamID_].viewDirection;
    }

    /**
     * @return Selected Camera Projector Matrix
     *
     * Equivalent to:
     * PerspectiveMatrix(selectedCam) * ViewMatrix(selectedCam)
     */
    glm::mat4 getSelectedCameraViewProjectorMatrix() const {
        return glm::perspective(cameras_[selectedCamID_].FOV,
                                cameras_[selectedCamID_].aspectRatio, nearZ, farZ) *
                                getSelectedCameraViewMatrix();
    }

    /**
     * @param camID Camera ID
     * @return Projector Matrix of Camera ID
     *
     * Equivalent to:
     * PerspectiveMatrix(camID) * ViewMatrix(camID)
     */
    glm::mat4 getCameraViewProjectorMatrix(const int camID) const {
        return glm::perspective(cameras_[camID].FOV, cameras_[camID].aspectRatio, nearZ, farZ) *
                 glm::lookAt(cameras_[camID].position,
                             cameras_[camID].position+cameras_[camID].viewDirection,
                             UP_);
    }


    /**
     * @return Selected Camera 180 FOV Projector Matrix for Full Coverage Feature
     */
    glm::mat4 getSelectedCamera180FOVProjectorMatrix() const {
        const static float AP_180 = 1.0f;
        const static float FOV_180 = 168.0f;
        const static glm::vec3 FOV_180_VIEW_DIRECTION = glm::vec3(0.0f, -1.0f, -0.001f);

        return glm::perspective(FOV_180, AP_180, nearZ, farZ) *
                glm::lookAt(getSelectedCameraPosition(),
                            getSelectedCameraPosition() + FOV_180_VIEW_DIRECTION,
                            UP_);
    }

    /**
     * @return Number of Camera(s) currently in container
     */
    int size() const {
        return cameras_.size();
    }

    /**
     * @return Whether the container is empty
     */
    bool empty() const {
        return cameras_.empty();
    }

    /**
     * @return Beginning const iterator of the container
     */
    QMap<int, Camera>::const_iterator cbegin() {
        return cameras_.cbegin();
    }

    /**
     * @return End const iterator of the container
     */
    QMap<int, Camera>::const_iterator cend() {
        return cameras_.cend();
    }

public slots:
    /**
     * @brief Sets new selected camera
     * @param camID Camera ID of new selected Camera.
     */
    void setSelectedCamera(int camID);

    /**
     * @brief Sets new FOV of selected Camera
     * @param value New FOV value.
     */
    void setSelectedCameraFOV(int value);

    /**
     * @brief Sets new Aspect Ratio of selected Camera
     * @param value New Aspect Ratio value.
     */
    void setSelectedCameraAspectRatio(double value);

    /**
     * @brief Sets new name of selected Camera
     * @param camName New Camera name.
     */
    void setSelectedCameraName(QString camName);

    /**
     * @brief Deletes selected Camera from container
     */
    void deleteSelectedCamera();

    /**
     * @brief Sets memento for future undo operations
     * Stores a snapshot of Cameras' container state.
     */
    void setMemento();

    /**
     * @brief Gets Memento to undo previous operations
     * Retrieves snapshot from Cameras' previous container state.
     */
    void getMemento();


signals:
    /**
     * @brief This signal is emitted when a new Camera is added
     * @param camName Camera name.
     * @param camID Camera ID.
     */
    void cameraAdded(int camID, QString camName);

    /**
     * @brief This signal is emitted to updates the selected
     * Camera's information displayed in Main Window
     */
    void setSelectedCameraPropertiesInMainWindow(int FOV, float aspectRatio, QString cameraName);

    /**
     * @brief This signal is emitted when the Camera's name is modified
     */
    void cameraNameModified(int camID, QString cameraName);

    /**
     * @brief This signal is emitted when Camera is deleted
     */
    void cameraDeleted(int camID);

private:
    /**
     * @brief Constructur is private to disable public instantiation
     */
    Cameras();

    /**
     * @brief Adds camera resources
     * @param camID Camera ID.
     */
    void addCameraResources(const int camID);

    /**
     * @brief Deletes Camera resources
     * @param camID Camera ID.
     */
    void deleteCameraResources(const int camID);

    /**
     * @brief Draws selected Camera's Z-Buffer to update its depth texture
     */
    void drawSelectedCameraZBuffer();

    /**
     * @brief Draws all Camera Z-Buffers to update their depth texture
     */
    void drawAllCameraZBuffers();

    /**
     * @brief Gets Camera ID for undo operation
     * @param currentState Current state of Cameras container.
     * @return Camera ID of Camera for undo operatio.
     */
    int getCamIDForUndoOperation(QMap<int, Camera> &newState);

    /**
     * @brief Unique singleton instance
     */
    static std::shared_ptr<Cameras> singletonInstance_;

    QMap<int, Camera> cameras_;
    QStack<CamerasMemento> mementoStack_;

    std::unique_ptr<CamerasZBuffer> camerasZBuffer_;
    std::unique_ptr<CameraLabels> cameraLabels_;
    std::unique_ptr<CameraViewFrustum> frustum_;
    std::unique_ptr<Camera3DModel> camera3DModel_;

    const glm::vec3 UP_;
    int countCameras_;
    int selectedCamID_;
    bool updateCameraView_;
    bool camera180FOVViewOn_;
    float camViewAspectRatio_;

    float maxCameraYPos_;

    const float nearZ;
    const float farZ;
    const int MAX_NUM_CAMERAS_;
};

#endif // CAMERAS_H
