#ifndef CAMERA3DMODEL_H
#define CAMERA3DMODEL_H
#include <glm/glm.hpp>

#include "model.h"
#include "glsl3dcameraprogram.h"

/**
 * @brief Draws a 3D Model Camera
 *
 * This class is used to draw a 3D camera Model at a specified
 * position and view direction.
 *
 * This class uses the same context of MainView as the
 * 3D camera models are drawn in MainView.
 *
 */

class Camera3DModel {
public:
    /**
     * @brief Constructor
     * @param context Pointer to QGLContext created in the MainWindow's MainView QGLWidget.
     */
    explicit Camera3DModel(QGLContext *context);

    /** @brief Copying is not allowed*/
    Camera3DModel(const Camera3DModel&) = delete;

    /** @brief Copying is not allowed*/
    Camera3DModel& operator=(const Camera3DModel&) = delete;

    /** @brief Destructor*/
    ~Camera3DModel() = default;

    /**
     * @brief Releases OpenGL context resources and restarts state of object
     */
    void clear();

    /**
     * @brief Draws the 3D camera Model in the MainView QGLWidget
     * @param cameraM Vector of Model matrices (i.e. world position) for all
     * Camera instances currently existing in MainView.
     * @param MVP Model View Projection matrix of MainView.
     */
    void draw(const QVector<glm::mat4> &camerasM, const glm::mat4 &MVP);

private:
    GLuint vaoCamModel_;    //verretx array object to draw 3D Camera
    Model cameraModel_;
    GLSL3DCameraProgram shaderProgram_;
    QGLContext *openGLDrawingContext_;

    const glm::mat4 O_; //Object space matrix that corrects the camera's default orientation
    glm::mat4 M_;       //Model matrix
    glm::mat4 MVP_;     //Model-View-Projection matrix
};

#endif // CAMERA3DMODEL_H
