#ifndef CAMERAVIEWFRUSTUM_H
#define CAMERAVIEWFRUSTUM_H

#include <array>

#include <glm/glm.hpp>

#include "glslfrustumprogram.h"
#include "model.h"

/**
 * @brief Struct that holds the values of the eight vertices of a cube in clip space.
 *
 * The cube's corners are from -1.0 to 1.0 in each of the 3 dimensions x, y, and z, and
 * its center is at the origin (0.0, 0.0).
 */
struct ClipSpaceCube {

    ClipSpaceCube() {
        vertices[0] = glm::vec4(-1.0f, 1.0f, -1.0f, 1.0f);  //far top left
        vertices[1] = glm::vec4(1.0f, 1.0f, -1.0f, 1.0f);   //far top right
        vertices[2] = glm::vec4(-1.0f, -1.0f, -1.0f, 1.0f); //far bottom left
        vertices[3] = glm::vec4(1.0f, -1.0f, -1.0f, 1.0f);  //far bottom right
        vertices[4] = glm::vec4(-1.0f, 1.0f, 1.0f, 1.0f);   //near top left
        vertices[5] = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);    //near top right
        vertices[6] = glm::vec4(-1.0f, -1.0f, 1.0f, 1.0f);  //near bottom left
        vertices[7] = glm::vec4(1.0f, -1.0f, 1.0f, 1.0f);   //near bottom right
    }
    enum {NUM_CUBE_VERTICES = 8};
    std::array<glm::vec4, NUM_CUBE_VERTICES> vertices;
};


/**
 * @brief Draws a transparent frustum representing Camera's coverage area
 */
class CameraViewFrustum {
public:
    /**
     * @brief Constructor
     * @param maxCameraYPos Maximum Y-coordinate point in MainView model.
     * @param col Color of trasparent frustum.
     */
    CameraViewFrustum(QGLContext *context, const float maxModelYPos, const glm::vec4 &col);

    /** @brief Copying is not allowed*/
    CameraViewFrustum(const CameraViewFrustum&) = delete;

    /** @brief Copying is not allowed*/
    CameraViewFrustum& operator=(const CameraViewFrustum&) = delete;

    /** @brief Destructor */
    ~CameraViewFrustum() = default;

    /**
     * @brief Releases OpenGL context resources and restarts state of object
     */
    void clear();

    /**
     * @brief Changes the frustum color to the one provided
     * @param frustumColor New color of frustum.
     */
    void setFrustumColor(const glm::vec4 &frustumColor);

    /**
     * @brief Sets/Updates frustum attributes. Typically called prior to calling drawFrustum()
     * @param fov Field-of-View of Camera.
     * @param ap Aspect Ratio of Camera.
     * @param VP View Perspictive matrix of Camera.
     */
    void setFrustumProperties(const int fov, const float ap, const glm::mat4 &VP);

    /**
     * @brief Draws frustum
     * @param MVP Model View Projection matrix of MainView.
     * @param texDepthID Texture ID of Z-Buffer (i.e. Depth) texture for shadow mapping.
     */
    void drawFrustum(const glm::mat4 &MVP, const GLuint texDepthID);

private:
    /**
     * @brief Builds frustum
     */
    void buildFrustum();

    /**
     * @brief Updates frustum (called per frame)
     * @param VP View Perspictive matrix.
     */
    void updateFrustum(const glm::mat4 &VP);

    QGLContext *openGLDrawingContext_;

    GLSLFrustumProgram shaderProgram_;
    GLuint vaoID_;
    GLuint vboVerticesID_;
    GLuint vboIndicesID_;
    glm::vec4 color_;

    float FOV_;
    float aspectRatio_;

    static const int NUM_FRUSTUM_VERTICES_ = 8;
    std::array<glm::vec4, NUM_FRUSTUM_VERTICES_> frustumVertices_;
    const ClipSpaceCube CLIP_SPACE_CUBE_;

    glm::mat4 MVP_; //Model View Projection matrix of MainView
    glm::mat4 cameraProjectorMatrix_;
};

#endif // CAMERAVIEWFRUSTUM_H
