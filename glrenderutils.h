#ifndef GLRENDERUTILS_H
#define GLRENDERUTILS_H

#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>

namespace MainModel {

const glm::vec4 skyColor = glm::vec4(0.5f, 0.5f, 1.0f, 1.0f);

const glm::mat4 M = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, 0.0f)) *
                    glm::rotate(glm::mat4(1.0f), 0.0f, glm::vec3(0.0f, 1.0f, 0.0f)) *
                    glm::scale(glm::mat4(1.0f), glm::vec3(1.0f, 1.0f, 1.0f));

const glm::vec3 lightPosition = glm::vec3(0.0f, 200.0f, 0.0f);
}


namespace GLUtilities {

    const glm::mat4 coordSpaceToTextureSamplingAdapter = glm::translate(glm::vec3(0.5f)) * glm::scale(glm::vec3(0.5f));
}

#endif // GLRENDERUTILS_H
