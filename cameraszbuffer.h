#ifndef CAMERASZBUFFER_H
#define CAMERASZBUFFER_H
#include <GL/glew.h>
#include <GL/GL.h>
#include <GL/GLU.h>

#include <memory>
#include <QGLContext>

#include "model.h"
#include "glslzbufferprogram.h"


class MainView;

/**
 * @brief ZBuffer texture container/manager for Camera(s)
 *
 * This class stores and manages resources for drawing Z-Buffer (i.e. depth buffer)
 * textures for each Camera in the Cameras class.
 *
 * The Z-Buffer textures are used to accomplish shawdow mapping when displaying the Cameras' coverage.
 *
 * Two textures are drawn for each Camera:
 *
 * -# Standard depth texture for the Camera's View.
 * -# 180 FOV depth texture that is used to show Full Coverage of PTZ Cameras.
 *
 * This class is used to:
 *
 * -# Add and delete Z-Buffer textures.
 * -# Draw Z-Buffer textures on a dedicated framebuffer.
 * -# Query generated Z-Buffer textures for use in shaders.
 */
class CamerasZBuffer {
public:
    /** @brief Constructor*/
    explicit CamerasZBuffer(MainView *glView);

    /** @brief Copying is not allowed*/
    CamerasZBuffer(const CamerasZBuffer&)  = delete;

    /** @brief Copying is not allowed*/
    CamerasZBuffer& operator=(const CamerasZBuffer&) = delete;

    /** @brief Destructor*/
    ~CamerasZBuffer() = default;

    /**
     * @brief Releases OpenGL context resources and restarts state of object
     */
    void clear();

    /**
     * @brief Generates new Z-Buffer texture for Camera camID
     * @param camID Camera ID.
     */
    GLuint generateCameraViewZBuffer(int camID);

    /**
     * @brief Generates new 180 FOV Z-Buffer texture for Camera camID
     * @param camID Camera ID.
     */
    GLuint generateCameraView180FOVZBuffer(int camID);

    /**
     * @brief Gets Camera View Z-Buffer texture ID for Camera [camID]
     * @param camID Camera ID.
     */
    GLuint getCameraViewZBuffer(int camID) const {
        return texCameraViewZBuffer_[camID];
    }

    /**
     * @brief Gets Camera 180 FOV Z-Buffer texture ID for Camera [camID]
     * @param camID Camera ID.
     */
    GLuint getCamera180FOVZBuffer(int camID) const {
        return texCamera180FOVZBuffer_[camID];
    }

    /**
     * @brief Draws Camera view Z-Buffer Texture
     * @param camID Camera ID.
     * @param cameraViewProjectorMatrix Camera view projector matrix.
     */
    void drawCameraViewFrameBuffer(int camID,
                                   const glm::mat4 &cameraViewProjectorMatrix) {
        renderTexZBuffer(texCameraViewZBuffer_[camID], cameraViewProjectorMatrix);
    }

    /**
     * @brief Draws Camera 180 FOV Z-Buffer Texture
     * @param camID Cameera ID.
     * @param cameraView180FOVProjectorMatrix Camera 180 FOV projector matrix.
     */
    void drawCamera180FOVFrameBuffer(int camID,
                                     const glm::mat4 &cameraView180FOVProjectorMatrix) {
        renderTexZBuffer(texCamera180FOVZBuffer_[camID], cameraView180FOVProjectorMatrix);
    }

    /**
     * @brief Deletes textures belonging to camID
     * @param camID Camera ID.
     */
    void deleteTexture(int camID);

private:
    /**
     * @brief Creates new Z-Buffer texture
     * @param textureID Texture ID. Must be same as Camera ID.
     */
    void setTexZBuffer(GLuint &textureID) const;

    /**
     * @brief Renders Z-Buffer texture
     * @param texID Texture ID.
     * @param projMat Projector matrix.
     */
    void renderTexZBuffer(GLuint &texID, const glm::mat4 &projectorVPMatrix) const;

    /**
     * @brief Deletes and releases texture resources belonging to camID
     * @param camID Camera ID.
     */
    void deleteTextureFromContext(int camID);

    const int TEXTURE_WIDTH_;
    const int TEXTURE_HEIGHT_;

    GLuint fboCameraViewZBuffer_;
    GLuint vaoID_;

    QMap<int, GLuint> texCameraViewZBuffer_;
    QMap<int, GLuint> texCamera180FOVZBuffer_;

    QGLContext *openGLDrawingContext_;
    GLSLZBufferProgram shaderProgram_;
    Model &mainViewModel_;

};

#endif // CAMERASZBUFFER_H
