#ifndef MAINVIEW_H
#define MAINVIEW_H

#include <GL/glew.h>
#include <GL/GL.h>
#include <GL/GLU.h>

#include <memory>
#include <QGLWidget>
#include <QDropEvent>
#include <QDragEnterEvent>
#include <QDragMoveEvent>

#include <opencv2/highgui/highgui.hpp>

#include "view.h"
#include "model.h"
#include "cameras.h"
#include "cameraview.h"
#include "glslprogram.h"
#include "glslmainviewprogram.h"

/**
 *@brief Main OpenGL screen showing the 3D World and coverage of all Camera instances
 *
 * This class is used to:
 *
 * -# Display the main 3D Model that represents the world in which Camera(s) are added.
 * -# Show Camera(s) coverage.
 * -# Show the positions of Camera(s).
 * -# Show the Labels of Camera(s).
 * -# Place Camera(s) in the 3D world by dragging the camera icon into MainView.
 * -# Change the position of Camera(s).
 */
class MainView : public QGLWidget {
    Q_OBJECT
public:
    /**
     * @brief Constructor
     * @param format Defined QGLFormat specifying the display format of the MainView OpenGL context.
     * @param parent Parent widget if any.
     * @param mFilePath Full file path of the 3D world model (.obj format).
     */
    MainView(const QGLFormat &format, const QString &mFilePath = "", QWidget *parent = 0);

    /** @brief Copying is not allowed*/
    MainView(const MainView&) = delete;

    /** @brief Copying is not allowed*/
    MainView& operator=(const MainView&) = delete;

    /** @brief Destructor*/
    ~MainView();

    /**
     * @brief Gets 3D world Model. Provided to allow sharing of resources.
     */
    Model& getModel() {
        return mainModel_;
    }

public slots:
    /**
     * @brief Enables/Disables "Show Full Coverage" feature of selected Camera
     * @param toggleVal True to enable "Show Full Coverage", false to disable feature.
     */
    void toggelShowCamera180FOVView(bool toggleVal);

    /**
     * @brief Sets the MainView View positon in world to Camera[camID]'s position
     * @param camID Camera ID.
     */
    void setMainViewWorldPosition(int camID);

signals:
    /**
     * @brief This signal is emitted when the right button of the mouse is pressed.
     *
     * This allows moving the position of the Camera in the world..
     */
    void mouseRightButtonPressed();

protected:
    void initializeGL();
    void paintGL();
    void resizeGL(int w, int h);
    bool event(QEvent *event);
    void keyPressEvent(QKeyEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
    void mousePressEvent(QMouseEvent *event);
    void dropEvent(QDropEvent *event);
    void dragEnterEvent(QDragEnterEvent *event);
    void dragMoveEvent(QDragMoveEvent *event);

private:
   glm::vec3 getMouseCursorWolrdPosition(float winPosX, float winPosY);
   void renderLater();

   bool pendingUpdate_;
   QString modelFilePath_;

   GLuint vaoMainViewID_;

   GLSLMainViewProgram shaderProgram_;
   Model mainModel_;

   std::unique_ptr<View> view_;
   std::shared_ptr<Cameras> cameras_;

   int mainViewWidth_;
   int mainViewHeight_;

   const float mainViewFOV_;

   bool cameraOn_;
   bool changeScene_;
   bool camera180FOVView_;

   glm::mat4 M_;    //Model matrix
   glm::mat4 V_;    //View matrix
   glm::mat4 P_;    //Perspective projection matrix
   glm::mat4 MV_;   //Model-View matrix
   glm::mat4 MVP_;  //Model-View-Projection matrix
};

#endif // MAINVIEW_H
