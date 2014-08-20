#ifndef GLSLCAMERAVIEWPROGRAM_H
#define GLSLCAMERAVIEWPROGRAM_H

#include "glslprogram.h"

/**
 * @brief GLSLProgram used by CameraView to render into its QGLWidget
 */
class GLSLCameraViewProgram final : public GLSLProgram {
public:
    /** @brief Default Constructor*/
    GLSLCameraViewProgram() {}

    /** @brief Copying is not allowed*/
    GLSLCameraViewProgram(const GLSLCameraViewProgram&) = delete;

    /** @brief Copying is not allowed*/
    GLSLCameraViewProgram& operator=(const GLSLCameraViewProgram&) = delete;

    /**
     * @brief Defines implementation to build the CameraView program object
     */
    virtual void buildShaderProgram() override;
};

#endif // GLSLCAMERAVIEWPROGRAM_H
