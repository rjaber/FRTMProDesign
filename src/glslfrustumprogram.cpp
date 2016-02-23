#include "glslfrustumprogram.h"

void GLSLFrustumProgram::buildShaderProgram() {
    loadShader(GL_VERTEX_SHADER, ":/shaders/frustumshader.vert");
    loadShader(GL_FRAGMENT_SHADER, ":/shaders/frustumshader.frag");
    createAndLinkProgram();
    use();
        addAttribute("vVertex");
        addAttribute("vColor");
        addUniform("MVP");
        addUniform("projectionMat");
        addUniform("shadowMap");
        addUniform("frustumColor");
        addUniform("yClipPlane");
        glUniform1i((*this)("shadowMap"), 0);
    unUse();
}
