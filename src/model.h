#ifndef MODEL_H
#define MODEL_H
#include <GL/glew.h>
#include <GL/gl.h>
#include <GL/glu.h>

#include <QVector>
#include <QString>
#include <QHash>
#include <QGLContext>

#include "glslprogram.h"
#include "objloader.h"

/**
 * @brief Manages a .obj 3D model to draw on an OpenGL screen
 *
 * This class is used to:
 *
 * -# Draw a .obj 3D model in a OpenGL window.
 * -# Load Vertex attributes (position, normal, tex coordinates)
 * into Vertex Buffer Objects and associates them with a GLSLProgram.
 * -# Share VBO resources with more than one OpenGL context by generating
 * a unique VAO per context.
 */

class Model {
public:
    /** @brief Default Constructor*/
    Model() = default;

    /** @brief Copying is not allowed*/
    Model(const Model&) = delete;

    /** @brief Copying is not allowed*/
    Model& operator=(const Model&) = delete;

    /** @brief Defualt Destructor*/
    ~Model() = default;

    /**
     * @brief Gets the maximum Y-coordinate point in the loaded .obj model.
     */
    float getModelYMax() const {
        return yMax_;
    }

    /**
     * @brief Deletes VAO from OpenGL context
     * @param context A Valid QGLContext.
     */
    void deleteVAOFromContext(QGLContext* context);

    /**
     * @brief Deletes Model Resources and data loaded from calling buildCompleteShaderModel()
     */
    void deleteModelResources();

    /**
     * @brief Loads all Vertex attributes data and vertex indices into vertex buffer objects
     * @param modelFilePath File path of .obj file.
     */
    void buildCompleteShaderModel(const QString &modelFilePath);

    /**
     * @brief Gets vertex array object ID for an OpenGL context. Creates one if it doesn't already exist.
     * @param shader GLSLProgram used to render 3D Model.
     * @param context A Valid  QGLContext.
     *
     * Vertex shader used with returned VAO must use the following attribute locations
     * for the three vertex attributes:
     *
     * @code
     * layout(location = 0) in vec3 vVertex;	//vertex position
     * layout(location = 1) in vec3 vNormal;	//vertex normal
     * layout(location = 2) in vec2 vUV;        //vertex uv coordinates
     * @endcode
     */
    GLuint getVAOForContext(GLSLProgram &shader, QGLContext *context);

    /**
     * @brief Draws Model in an OpenGL window.
     */
    void drawModel() const;

    /**
     * @brief Draws Model in an OpenGL window. This overloaded function is used
     * when textures of the model needs to be bound to shader samplers.
     * @param shader GLSLProgram used to render 3D Model
     */
    void drawModel(GLSLProgram &shader) const;

private:
    /**
     * @brief Loads model data from .obj file into corresponding structs and containers
     * @param objFullFilePath File path of .obj file.
     */
    void loadModel(const QString &objFullFilePath);

    /**
     * @brief Finds maximum Y-coordinate in loaded .obj model
     */
    void findModelYMax();

    void loadModelTextures(const QString &modelFilePath);

    ObjLoader objLoader_;

    GLuint vaoID_;
    GLuint vboVerticesID_;
    GLuint vboIndicesID_;

    QVector<Mesh> meshes_;
    QVector<Material> materials_;
    QVector<unsigned short> indices_;
    QVector<Vertex> vertices_;
    QVector<GLuint> textures_;

    float yMax_;

    QHash<QGLContext*, GLuint> vaoHashContainer_;
};

#endif // MODEL_H
