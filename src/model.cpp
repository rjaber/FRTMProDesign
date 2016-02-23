#include "model.h"

#include <algorithm>
#include <QDebug>
#include <QImage>
#include <QStringList>

#include <glm/gtc/type_ptr.hpp>

#include "frtmutils.h"


void Model::deleteModelResources() {
    meshes_.clear();
    materials_.clear();
    indices_.clear();
    vertices_.clear();

    for (auto& texture : textures_) {
        glDeleteTextures(1, &texture);
    }
    textures_.clear();

    glDeleteBuffers(1, &vboVerticesID_);
    glDeleteBuffers(1, &vboIndicesID_);
}

void Model::deleteVAOFromContext(QGLContext* context) {
    auto i = vaoHashContainer_.find(context);
    if (vaoHashContainer_.find(context) != vaoHashContainer_.end()) {
        context->makeCurrent();
        glDeleteVertexArrays(1, &i.value());
    }
}

void Model::findModelYMax() {
    auto itrYMax = std::max_element(vertices_.begin(), vertices_.end(),
                                   [] (Vertex &v1, Vertex &v2) { return v1.pos.y < v2.pos.y; });
    yMax_ = itrYMax->pos.y;
}

void Model::buildCompleteShaderModel(const QString &modelFilePath) {

    loadModel(modelFilePath);
    findModelYMax();

    glGenBuffers(1, &vboVerticesID_);

    glBindBuffer (GL_ARRAY_BUFFER, vboVerticesID_);
    glBufferData (GL_ARRAY_BUFFER, sizeof(Vertex) * vertices_.size(), &(vertices_[0].pos.x), GL_STATIC_DRAW);

    if (materials_.size()==1) {
        glGenBuffers(1, &vboIndicesID_);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vboIndicesID_);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLushort) * indices_.size(), &(indices_[0]), GL_STATIC_DRAW);
    }

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,0);
}

GLuint Model::getVAOForContext(GLSLProgram &shader, QGLContext *context){

    if (!vaoHashContainer_.contains(context)) {

        glGenVertexArrays(1, &vaoID_);
        glBindVertexArray(vaoID_);

        shader.use();
            glBindBuffer (GL_ARRAY_BUFFER, vboVerticesID_);

            glEnableVertexAttribArray(shader["vVertex"]);
            glVertexAttribPointer(shader["vVertex"], 3, GL_FLOAT, GL_FALSE,sizeof(Vertex),0);

            if (shader.isActive(shader["vNormal"])) {
                glEnableVertexAttribArray(shader["vNormal"]);
                glVertexAttribPointer(shader["vNormal"], 3, GL_FLOAT,
                        GL_FALSE, sizeof(Vertex), (const GLvoid*)(offsetof(Vertex, normal)) );
            }

            if (shader.isActive(shader["vUV"])) {
                glEnableVertexAttribArray(shader["vUV"]);
                glVertexAttribPointer(shader["vUV"], 2, GL_FLOAT,
                        GL_FALSE, sizeof(Vertex), (const GLvoid*)(offsetof(Vertex, uv)) );
            }

            if (materials_.size()==1)
                glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vboIndicesID_);

            glBindVertexArray(0);
            glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,0);

            vaoHashContainer_.insert(context, vaoID_);
        shader.unUse();
    }

    return vaoHashContainer_[context];
}

void Model::drawModel() const {
    if (materials_.size()==1) {
        glDrawElements(GL_TRIANGLES, indices_.size(), GL_UNSIGNED_SHORT, 0);
    }
    else {
        for (const auto& material : materials_) {
            glDrawElements(GL_TRIANGLES, material.count, GL_UNSIGNED_SHORT,
                           (const GLvoid*)(&indices_[material.offset]));
        }
    }
}

void Model::drawModel(GLSLProgram &shader) const {

    for (size_t i=0; i<materials_.size(); ++i) {

        if (materials_[i].map_Kd != "") {
            glUniform1f(shader("useTextureMap"), 1.0); //use texture
            GLint whichID[1];
            glActiveTexture(GL_TEXTURE0);
            glGetIntegerv(GL_TEXTURE_BINDING_2D, whichID);
            if (whichID[0] != textures_[i]) {
                glBindTexture(GL_TEXTURE_2D, textures_[i]);
            }
        }
        else {
            glUniform1f(shader("useTextureMap"), 0.0);
            glUniform3fv(shader("materialKd"), 1 , glm::value_ptr(materials_[i].Kd));

            if (shader.isActive(shader("materialKs")))
                glUniform3fv(shader("materialKs"), 1 , glm::value_ptr(materials_[i].Ks));

            if (shader.isActive(shader("materialKa")))
                glUniform3fv(shader("materialKa"), 1 , glm::value_ptr(materials_[i].Ka));

            if (shader.isActive(shader("materialNs")))
                glUniform1f(shader("materialNs"), materials_[i].Ns);
        }

        if (materials_.size() == 1)
            glDrawElements(GL_TRIANGLES, indices_.size(), GL_UNSIGNED_SHORT, 0);
        else
            glDrawElements(GL_TRIANGLES, materials_[i].count, GL_UNSIGNED_SHORT,
                           (const GLvoid*)(&indices_[materials_[i].offset]));
    }
}

void Model::loadModel(const QString &objFullFilePath) {
    if (!objLoader_.load(objFullFilePath, meshes_, vertices_, indices_, materials_)) {
        qDebug() << "Obj Model Failed to Load";
        exit(EXIT_FAILURE);
    }

    loadModelTextures(FRTMUtils::getModelFilePath(objFullFilePath));
}

void Model::loadModelTextures(const QString &modelFilePath) {

    for (auto& material : materials_) {
        if (material.map_Kd != "") {
            GLuint id = 0;
            glGenTextures(1, &id);
            glBindTexture(GL_TEXTURE_2D, id);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

            const QString textureFileName = modelFilePath + material.map_Kd;
			QImage image(textureFileName);
			image.convertToFormat(QImage::Format_RGB32);

            if (image.isNull()) {
                qDebug() << "Image " << textureFileName <<" is empty";
                material.map_Kd = "";
                continue;
            }

			image = image.mirrored();

            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, image.width(), image.height(),
                         0, GL_RGBA, GL_UNSIGNED_BYTE, image.bits());

            textures_.push_back(id);
        }
    }
}

