#ifndef GLSLZBUFFERPROGRAM_H
#define GLSLZBUFFERPROGRAM_H
#include "glslprogram.h"

/**
 * @brief GLSLProgram used by CamerasZBuffer to render Z-Buffer (i.e. depth) textures into a dedicated framebuffer
 */
class GLSLZBufferProgram final : public GLSLProgram {
public:
    /** @brief Default Constructor*/
    GLSLZBufferProgram() {}

    /** @brief Copying is not allowed*/
    GLSLZBufferProgram(const GLSLZBufferProgram&) = delete;

    /** @brief Copying is not allowed*/
    GLSLZBufferProgram& operator=(const GLSLZBufferProgram&) = delete;

    /**
     * @brief Defines implementation to build the CamerasZBuffer program object
     */
    virtual void buildShaderProgram() override;
};

#endif // GLSLZBUFFERPROGRAM_H
