#ifndef CAMERALABELS_H
#define CAMERALABELS_H
#include <QGLWidget>
#include <QLabel>
#include <QMouseEvent>
#include <QAction>

#include <glm/glm.hpp>


class MainView;

/**
 * @brief Camera's Label
 *
 * This class inherits from QLabel and adds two features that:
 *
 * -# Allow the selection of Camera(s) by left-clicking the Camera's label.
 * -# Enables the GoTo camera Action.
 *
 * @ToDo Use something other than QLabel to display Camera's label.
 */
class Label : public QLabel {
    Q_OBJECT
public:
    /**
     * @brief Constructor
     * @param lblID Label Unique ID of label. Must Match Camera ID.
     * @param glMainWidget Pointer to MainWindow's MainView QGLWidget.
     */
    Label(int lblID, const MainView *glMainWidget);

    /** @brief Copying is not allowed*/
    Label(const Label&) = delete;

    /** @brief Copying is not allowed*/
    Label& operator=(const Label&) = delete;

    /** @brief Destructor*/
    ~Label() = default;

signals:
    /**
     * @brief This signal is emitted when the Label is clicked in the MainView
     * @param labelID Unique ID of Label.
     *
     * This signal changes the selected Camera of the Cameras class to the one clicked.
     */
    void labelClicked(int labelID);

    /**
     * @brief This signal is emitted when the goToCameraView Action is triggered
     * @param labelID Unique ID of Label.
     *
     * This signal is sent to MainView to change its view to the view of the Camera of labelID.
     */
    void goToCameraView(int labelID);

private slots:
    /**
     * @brief Called when the goToCameraView Action is triggered
     */
    void goToCameraViewActionTriggered();

protected:
    /**
     * @brief Redefines the mousePressEvent for Label
     */
    void mousePressEvent(QMouseEvent *event);

private:
    /**
     * @brief Creates the GoTo camera Action
     */
    void createGoToCameraAction();

    const int LABEL_ID_;    //Must be the same as the camID of the Camera it represents
    QAction *goToCameraViewAction_;
};


/**
 * @brief Camera Label(s) Container Manager
 *
 * Provides a container wrapper to store Label(s) of Camera(s).
 * Adds extra features in order to manage the Label(s) in the MainView QGLWidget.
 *
 * This class is used to:
 *
 * -# Add and delete Label(s).
 * -# Draw Label(s) in MainView.
 */
class CameraLabels {
public:
    /** @brief Constructor*/
    explicit CameraLabels(MainView *glWidget) {
        mainViewGLWidget_ = glWidget;
    }

    /** @brief Copying is not allowed*/
    CameraLabels(const CameraLabels&) = delete;

    /** @brief Copying is not allowed*/
    CameraLabels& operator=(const CameraLabels&) = delete;

    /** @brief Destructor*/
    ~CameraLabels() = default;

    /**
     * @brief Adds New Label for a Camera
     * @param labelID Value of Camera's ID.
     */
    void addNewLabel(int labelID);

    /**
     * @brief Deletes Label belonging to Camera (labelID)
     * @param labelID Value of Camera's ID.
     */
    void deleteLabel(int labelID);

    /**
     * @brief Draws all camera Label(s) in MainView QGLWidget
     * @param World position of every Camera instance currently existing in MainView.
     * @param V View matrix (i.e. lookAt matrix) of MainView.
     * @param P Perspective projection matrix of MainView.
     */
    void drawCameraLabels(const QVector<glm::vec3> &camPosition, const glm::mat4 &V, const glm::mat4 &P);

private:
    /**
     * @param labelScreenpos Sreen position of Camera's Label
     * @return Whether the Camera Label is inside the MainView window.
     */
    bool isLabelInsideWindow(const glm::vec3 &labelScreenPos) const;

    static const int LABEL_DIMENSION_ = 16;

    /** @brief Label(s) container*/
    QMap<int, Label*> cameraLabels_;

    /** @brief MainView QGLWidget where the Label(s) are drawn*/
    MainView *mainViewGLWidget_;
};

#endif // CAMERALABELS_H
