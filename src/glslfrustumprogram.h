#ifndef GLSLFRUSTUMPROGRAM_H
#define GLSLFRUSTUMPROGRAM_H
#include "glslprogram.h"

/**
 * @brief GLSLProgram used by CameraViewFrustum to render the Camera's transparent frustum in the MainView QGLWidget
 */
class GLSLFrustumProgram final: public GLSLProgram {
public:
    /** @brief Default Constructor*/
    GLSLFrustumProgram() {}

    /** @brief Copying is not allowed*/
    GLSLFrustumProgram(const GLSLFrustumProgram&) = delete;

    /** @brief Copying is not allowed*/
    GLSLFrustumProgram& operator=(const GLSLFrustumProgram&) = delete;

    /**
     * @brief Defines implementation to build the CameraViewFrustum program object
     */
    virtual void buildShaderProgram() override;
};

#endif // GLSLFRUSTUMPROGRAM_H
