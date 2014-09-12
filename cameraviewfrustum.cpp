#include "cameraviewfrustum.h"

#include <glm/gtx/transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "glslfrustumprogram.h"
#include "frtmutils.h"


CameraViewFrustum::CameraViewFrustum(QGLContext *context, float maxModelYPos , const glm::vec4 &col) :
    openGLDrawingContext_ (context),
    color_(col) {
    shaderProgram_.buildShaderProgram();
    shaderProgram_.use();
        glUniform4fv(shaderProgram_("frustumColor"), 1, glm::value_ptr(color_));
        const glm::vec4 Y_CLIP_PLANE = glm::vec4(0.0f, -1.0f, 0.0f, maxModelYPos);
        glUniform4fv(shaderProgram_("yClipPlane"), 1, glm::value_ptr(Y_CLIP_PLANE));
    shaderProgram_.unUse();

    buildFrustum();
}

void CameraViewFrustum::clear() {
    openGLDrawingContext_->makeCurrent();
    glDeleteBuffers(1, &vboVerticesID_);
    glDeleteBuffers(1, &vboIndicesID_);

    glDeleteVertexArrays(1, &vaoID_);
    shaderProgram_.deleteShaderProgram();
}

void CameraViewFrustum::setFrustumColor(const glm::vec4 &frustumColor) {
    color_ = frustumColor;
    shaderProgram_.use();
        glUniform4fv(shaderProgram_("frustumColor"), 1, glm::value_ptr(color_));
    shaderProgram_.unUse();
}

void CameraViewFrustum::setFrustumProperties(int fov, float ap, const glm::mat4 &VP) {
    FOV_ = static_cast<float>(fov);
    aspectRatio_ = ap;
    updateFrustum(VP);
}

void CameraViewFrustum::drawFrustum(const glm::mat4 &MVP, const GLuint texDepthID) {
    static glm::mat4 projectorMatrix;
    projectorMatrix = GLUtils::coordSpaceToTextureSamplingAdapter * cameraProjectorMatrix_;

    glBindVertexArray(vaoID_);
    shaderProgram_.use();
        MVP_ = MVP;
        glUniformMatrix4fv(shaderProgram_("MVP"), 1, GL_FALSE, glm::value_ptr(MVP_));
        glUniformMatrix4fv(shaderProgram_("projectionMat"), 1, GL_FALSE, glm::value_ptr(projectorMatrix));

        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, texDepthID);

        glEnable(GL_CLIP_PLANE0);
        static const int NUM_FRUSTUM_INDICES = 36;
        glDrawElements(GL_TRIANGLES, NUM_FRUSTUM_INDICES, GL_UNSIGNED_BYTE, 0);
    shaderProgram_.unUse();
}

void CameraViewFrustum::buildFrustum() {
    glGenVertexArrays(1, &vaoID_);
    glGenBuffers(1, &vboVerticesID_);
    glGenBuffers(1, &vboIndicesID_);

    glBindVertexArray(vaoID_);

    glBindBuffer(GL_ARRAY_BUFFER, vboVerticesID_);
    glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec4)* NUM_FRUSTUM_VERTICES_, NULL, GL_DYNAMIC_DRAW);

    glEnableVertexAttribArray(shaderProgram_["vVertex"]);
    glVertexAttribPointer(shaderProgram_["vVertex"], sizeof(glm::vec4)/sizeof(float), GL_FLOAT, GL_FALSE, 0, 0);

    glBindBuffer(GL_ARRAY_BUFFER, 0);

    const GLubyte indexData[] = {
        //two triangles per face
        0, 1, 2,    //far face triangle 1
        1, 3, 2,    //far face triangle 2
        4, 5, 6,    //near face triangle 1
        5, 7, 6,    //near face triangle 2
        1, 3, 5,    //right face triangle 1
        5, 3, 7,    //right face triangle 2
        0, 4, 2,    //left face triangle 1
        4, 6, 2,    //left face triangle 2
        0, 1, 4,    //top face triangle 1
        1, 5, 4,    //top face triangle 2
        2, 6, 3,    //bottom face triangle 1
        3, 6, 7     //bottom face triangle 2
    };

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vboIndicesID_);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indexData), indexData, GL_STATIC_DRAW);

    glBindVertexArray(0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,0);

}

void CameraViewFrustum::updateFrustum(const glm::mat4 &VP)  {
    cameraProjectorMatrix_ = VP;
    //calculates frustum vertices in world space
    for (size_t i = 0 ; i < NUM_FRUSTUM_VERTICES_ ; ++i){
        frustumVertices_[i] = glm::inverse(cameraProjectorMatrix_) * CLIP_SPACE_CUBE_.vertices[i];
        assert(frustumVertices_[i].w != 0);
        frustumVertices_[i] /= frustumVertices_[i].w;
    }

    glBindBuffer(GL_ARRAY_BUFFER, vboVerticesID_);
    glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(frustumVertices_), (GLfloat*)frustumVertices_.data());
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}
