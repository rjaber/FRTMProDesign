#ifndef GLSLMAINVIEWPROGRAM_H
#define GLSLMAINVIEWPROGRAM_H
#include "glslprogram.h"

/**
 * @brief GLSLProgram used by MainView to render into its QGLWidget
 */
class GLSLMainViewProgram final : public GLSLProgram {
public:
    /** @brief Default Constructor*/
    GLSLMainViewProgram() {}

    /** @brief Copying is not allowed*/
    GLSLMainViewProgram(const GLSLMainViewProgram&) = delete;

    /** @brief Copying is not allowed*/
    GLSLMainViewProgram& operator=(const GLSLMainViewProgram&) = delete;

    /**
     * @brief Defines implementation to build the MainView program object
     */
    virtual void buildShaderProgram() override;


};

#endif // GLSLMAINVIEWPROGRAM_H
