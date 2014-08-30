#include "glslcameraviewprogram.h"

void GLSLCameraViewProgram::buildShaderProgram() {
    loadShader(GL_VERTEX_SHADER, ":/shaders/cameraview.vert");
    loadShader(GL_FRAGMENT_SHADER, ":/shaders/cameraview.frag");
    createAndLinkProgram();
    use();
        addAttribute("vVertex");
        addAttribute("vNormal");
        addAttribute("vUV");
        addUniform("M");
        addUniform("MV");
        addUniform("MVP");
        addUniform("lightPosition");
        addUniform("useTextureMap");
        addUniform("materialKd");
        addUniform("materialKs");
        addUniform("materialKa");
        addUniform("materialNs");
        addUniform("textureMap");
        glUniform1i((*this)("textureMap"), 0);
    unUse();
}
