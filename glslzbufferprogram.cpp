#include "glslzbufferprogram.h"

void GLSLZBufferProgram::buildShaderProgram() {
    loadShader(GL_VERTEX_SHADER, ":/shaders/framebuffer.vert");
    loadShader(GL_FRAGMENT_SHADER, ":/shaders/framebuffer.frag");
    createAndLinkProgram();
    use();
         addAttribute("vVertex");
         addUniform("MVP");
    unUse();
}
