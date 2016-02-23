#ifndef FRTMUTILS_H
#define FRTMUTILS_H

#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>

class QString;
class QFile;
class QWidget;


namespace MainModel {

const glm::vec4 skyColor = glm::vec4(0.5f, 0.5f, 1.0f, 1.0f);

const glm::mat4 M = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, 0.0f)) *
                    glm::rotate(glm::mat4(1.0f), glm::radians(0.0f), glm::vec3(0.0f, 1.0f, 0.0f)) *
                    glm::scale(glm::mat4(1.0f), glm::vec3(1.0f, 1.0f, 1.0f));

const glm::vec3 lightPosition = glm::vec3(0.0f, 200.0f, 0.0f);
}


namespace GLUtils {

    const glm::mat4 coordSpaceToTextureSamplingAdapter = glm::translate(glm::vec3(0.5f)) * glm::scale(glm::vec3(0.5f));
}


namespace FRTMUtils {

    void setWidgetMinimumSize(QWidget *widget, float minWidthFactor, float minHeightFactor);
    QString getModelFilePath(const QString &objFilePath);
    void readFileErrorInfo(const QFile &file);

}

#endif // FRTMUTILS_H
