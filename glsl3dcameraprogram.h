#ifndef GLSL3DCAMERAPROGRAM_H
#define GLSL3DCAMERAPROGRAM_H

#include "glslprogram.h"

/**
 * @brief GLSLProgram used by Camera3DModel to draw 3D cameras in the MainView QGLWidget
 */

class GLSL3DCameraProgram final : public GLSLProgram {

public:
    /** @brief Default Constructor*/
    GLSL3DCameraProgram() {}

    /** @brief Copying is not allowed*/
    GLSL3DCameraProgram(const GLSL3DCameraProgram&) = delete;

    /** @brief Copying is not allowed*/
    GLSL3DCameraProgram& operator=(const GLSL3DCameraProgram&) = delete;

    /**
     * @brief Defines implementation to build the 3DCamera program object
     */
    virtual void buildShaderProgram() override;

};

#endif // GLSL3DCAMERAPROGRAM_H
