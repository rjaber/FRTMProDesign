#include "glslmainviewprogram.h"

void GLSLMainViewProgram::buildShaderProgram() {
    loadShader(GL_VERTEX_SHADER, ":/shaders/mainview.vert");
    loadShader(GL_FRAGMENT_SHADER, ":/shaders/mainview.frag");

    createAndLinkProgram();
    use();
        addAttribute("vVertex");
        addAttribute("vNormal");
        addAttribute("vUV");
        addUniform("MV");
        addUniform("M");
        addUniform("MVP");
        addUniform("projectionMat");
        addUniform("potentialProjMat");
        addUniform("textureMap");
        addUniform("useTextureMap");
        addUniform("materialKd");
        addUniform("materialKs");
        addUniform("materialKa");
        addUniform("materialNs");
        addUniform("lightPosition");
        addUniform("shadowMap[0]");
        addUniform("shadowMap[1]");
        addUniform("shadowMap[2]");
        addUniform("shadowMap[3]");
        addUniform("shadowMap[4]");
        addUniform("shadowMap[5]");
        addUniform("shadowMap[6]");
        addUniform("numOfCams");
        addUniform("is180FOVOn");
        addUniform("shadowMapPotential");
        addUniform("yClipPlane");
        //set values of uniform samplers
        glUniform1i((*this)("textureMap"), 0);
        glUniform1i((*this)("shadowMap[0]"), 1);
        glUniform1i((*this)("shadowMap[1]"), 2);
        glUniform1i((*this)("shadowMap[2]"), 3);
        glUniform1i((*this)("shadowMap[3]"), 4);
        glUniform1i((*this)("shadowMap[4]"), 5);
        glUniform1i((*this)("shadowMap[5]"), 6);
        glUniform1i((*this)("shadowMap[6]"), 7);
        glUniform1i((*this)("shadowMapPotential"), 8);        
    unUse();
}
