#include "glsl3dcameraprogram.h"

void GLSL3DCameraProgram::buildShaderProgram() {
    loadShader(GL_VERTEX_SHADER, ":/shaders/camera3Dmodel.vert");
    loadShader(GL_FRAGMENT_SHADER, ":/shaders/camera3Dmodel.frag");
    createAndLinkProgram();
    use();
        addAttribute("vVertex");
        addAttribute("vNormal");
        addAttribute("vUV");
        addUniform("MVP");
        addUniform("useTextureMap");
        addUniform("materialKd");
        addUniform("materialKs");
        addUniform("materialKa");
        addUniform("materialNs");
        addUniform("textureMap");
        glUniform1i((*this)("textureMap"), 0);
    unUse();
}
