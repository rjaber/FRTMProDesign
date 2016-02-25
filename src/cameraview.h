#ifndef CAMERAVIEW_H
#define CAMERAVIEW_H
#include <GL/glew.h>
#include <GL/gl.h>
#include <GL/glu.h>

#include <QGLWidget>

#include "glslcameraviewprogram.h"
#include "view.h"
#include "model.h"
#include "cameras.h"


class MainView;

/**
 *@brief OpenGL screen showing the selected Camera View.
 *
 * This class is used to:
 *
 * -# Show the selected Camera's coverage.
 * -# Change the view direction of the selected Camera.
 */
class CameraView : public QGLWidget {
    Q_OBJECT
public:
    /**
     * @brief Constructor
     * @param parent Parent widget if any.
     * @param mainViewGLWidget Pointer to MainWindow's MainView QGLWidget.
     */
    CameraView(MainView *mainViewGLWidget, QWidget *parent = 0);

    /** @brief Copying is not allowed*/
    CameraView(const CameraView&) = delete;

    /** @brief Copying is not allowed*/
    CameraView& operator=(const CameraView&) = delete;

    /** @brief Destructor*/
    ~CameraView();

signals:
    void mousePressed();

protected:
    void initializeGL() override;
    void paintGL() override;
    void resizeGL(int w, int h) override;
    bool event(QEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;
    void mousePressEvent(QMouseEvent *event) override;

private:
    void renderLater();
    void updateView();
    void updatePerspectiveView();
    void clearScreenBlack();

    std::unique_ptr<View> view_;
    std::shared_ptr<Cameras> cameras_;
    Model& model_;
    GLSLCameraViewProgram shaderProgram_;
    GLuint vaoID_;

    glm::mat4 M_;
    glm::mat4 V_;
    glm::mat4 P_;
    glm::mat4 MV_;
    glm::mat4 MVP_;

    bool pendingUpdate_;

};

#endif // CAMERAVIEW_H
