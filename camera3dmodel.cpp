#include "camera3dmodel.h"

#include <glm/gtx/transform.hpp>
#include <glm/gtc/type_ptr.hpp>


Camera3DModel::Camera3DModel(QGLContext *context) :
    openGLDrawingContext_(context),
    O_(glm::translate(glm::mat4(1.0f), glm::vec3(7.3f, 4.22f, 6.8f)) *
       glm::rotate(glm::mat4(1.0f),  157.5f, glm::vec3(1.0f, 0.0f, 0.0f)) *
       glm::scale(glm::mat4(1.0f), glm::vec3(3.0f, 3.0f, 3.0f))) {

    shaderProgram_.buildShaderProgram();
    cameraModel_.buildCompleteShaderModel(":/models/SecurityCamera.obj");
    vaoCamModel_ = cameraModel_.getVAOForContext(shaderProgram_, openGLDrawingContext_);
}

void Camera3DModel::clear() {
    cameraModel_.deleteVAOFromContext(openGLDrawingContext_);
    shaderProgram_.deleteShaderProgram();
    openGLDrawingContext_ = nullptr;
    vaoCamModel_ = 0;
}

void Camera3DModel::draw(const QVector<glm::mat4> &camerasM, const glm::mat4 &MVP) {

    glBindVertexArray(vaoCamModel_);
    shaderProgram_.use();
        //Draws all  3D Cameras in MainView
        for (const auto& camM : camerasM) {
            M_ = camM * O_;
            MVP_ = MVP * M_;
            glUniformMatrix4fv(shaderProgram_("MVP"), 1, GL_FALSE, glm::value_ptr(MVP_));
            cameraModel_.drawModel(shaderProgram_);
        }
    shaderProgram_.unUse();
}
