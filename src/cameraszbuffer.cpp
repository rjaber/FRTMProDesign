#include "cameraszbuffer.h"

#include <glm/gtx/transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "mainview.h"
#include "frtmutils.h"

CamerasZBuffer::CamerasZBuffer(MainView *glView) :
    TEXTURE_WIDTH_(400),
    TEXTURE_HEIGHT_(400),
    mainViewModel_(glView->getModel()){

    openGLDrawingContext_ = glView->context();

    shaderProgram_.buildShaderProgram();
    vaoID_ = mainViewModel_.getVAOForContext(shaderProgram_, openGLDrawingContext_);

    glGenFramebuffers(1, &fboCameraViewZBuffer_);
}

void CamerasZBuffer::clear() {
    openGLDrawingContext_->makeCurrent();

    for (const auto &texture : texCameraViewZBuffer_) {
        deleteTextureFromContext(texture);
    }

    texCameraViewZBuffer_.clear();
    texCamera180FOVZBuffer_.clear();
    shaderProgram_.deleteShaderProgram();

    glDeleteFramebuffers(1, &fboCameraViewZBuffer_);
    openGLDrawingContext_ = nullptr;

    fboCameraViewZBuffer_ = 0;
    vaoID_ = 0;
}

GLuint CamerasZBuffer::generateCameraViewZBuffer(int camID) {
    openGLDrawingContext_->makeCurrent();
    setTexZBuffer(texCameraViewZBuffer_[camID]);
    return texCameraViewZBuffer_[camID];
}

GLuint CamerasZBuffer::generateCameraView180FOVZBuffer(int camID) {
    openGLDrawingContext_->makeCurrent();
    setTexZBuffer(texCamera180FOVZBuffer_[camID]);
    return texCamera180FOVZBuffer_[camID];
}

void CamerasZBuffer::deleteTexture(int camID) {
    openGLDrawingContext_->makeCurrent();

    deleteTextureFromContext(camID);

    texCameraViewZBuffer_.remove(camID);
    texCamera180FOVZBuffer_.remove(camID);
}

void CamerasZBuffer::setTexZBuffer(GLuint &textureID) const {
    glGenTextures(1, &textureID);
    glBindTexture(GL_TEXTURE_2D, textureID);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, TEXTURE_WIDTH_,
                 TEXTURE_HEIGHT_, 0, GL_DEPTH_COMPONENT, GL_UNSIGNED_BYTE, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_MODE, GL_COMPARE_REF_TO_TEXTURE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_FUNC, GL_LEQUAL);
    glBindTexture(GL_TEXTURE_2D, 0);
}

void CamerasZBuffer::renderTexZBuffer(GLuint &texID, const glm::mat4 &projectorVPMatrix) const {
    openGLDrawingContext_->makeCurrent();

    const glm::mat4 MVP = projectorVPMatrix * MainModel::M;

    glBindFramebuffer(GL_FRAMEBUFFER, fboCameraViewZBuffer_);
    glBindVertexArray(vaoID_);
    shaderProgram_.use();
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT,
                               GL_TEXTURE_2D, texID, 0);

        glDrawBuffer(GL_NONE);
        glClear(GL_DEPTH_BUFFER_BIT);
        glEnable(GL_POLYGON_OFFSET_FILL);
        glPolygonOffset(2.0f, 4.0f);
        glViewport(0,0,TEXTURE_WIDTH_, TEXTURE_HEIGHT_);
        glUniformMatrix4fv(shaderProgram_("MVP"), 1, GL_FALSE, glm::value_ptr(MVP));
        glEnable(GL_CULL_FACE);

        mainViewModel_.drawModel();
    shaderProgram_.unUse();

    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    glBindTexture(GL_TEXTURE_2D, 0);
}

void CamerasZBuffer::deleteTextureFromContext(int camID) {

    glDeleteTextures(1, &texCameraViewZBuffer_[camID]);
    glDeleteTextures(1, &texCamera180FOVZBuffer_[camID]);
}


