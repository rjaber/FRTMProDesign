#ifndef OBJLOADER_H
#define OBJLOADER_H

#include <QString>
#include <QVector>
#include <QFile>

#include <glm/glm.hpp>

/**
 * @brief Struct for three vertex attibutes: position vector , normal vector, and texture coordinates
 */
struct Vertex {
    glm::vec3 pos;
    glm::vec3 normal;
    glm::vec2 uv;
};

/**
 * @brief Struct to store vertix attribute indices of a triangular face consisting of three Vertices
 */
struct Face {
    unsigned short	v1,vn1,vt1,     //index of vertex position, vertex normal, and vertex uv coord
                    v2,vn2,vt2,
                    v3,vn3,vt3;
};

/**
 * @brief Struct representing a mesh that is typically associated with a material in the .obj Model
 */
struct Mesh {
    Mesh() {
        materialIndex = -1;
    }
    QString			name;
    int				materialIndex;
};

/**
 * @brief Struct for all essential data defining a material in .mtl file and the vertex indices that use it
 */
struct Material {
    float Ns;
    float Ni;
    float d;
    float Tr;
    glm::vec3 Tf;
    int illum;
    glm::vec3 Ka;
    glm::vec3 Kd;
    glm::vec3 Ks;
    glm::vec3 Ke;
    QString map_Ka;
    QString map_Kd;
    QString name;

    QVector<unsigned short> subIndices;
    int offset;
    int count;
};

/**
 * @brief Loads a Wavefront .obj format 3D model into defined containers
 */
class ObjLoader {
public:
    /** @brief Default Constructor*/
    ObjLoader() = default;

    /** @brief Copying is not allowed*/
    ObjLoader(const ObjLoader&) = delete;

    /** @brief Copying is not allowed*/
    ObjLoader& operator=(const ObjLoader&) = delete;

    /**
     * @brief Parses .obj and .mtl files into containers of vertices, meshes, indices, and materials
     * @param fileName File path of .obj file.
     * @param meshes Out variable used to store meshes of the 3D model.
     * @param vertices Out variable used to store vertices of the 3D model.
     * @param indices Out variable used to store indices of the 3D model.
     * @param materials Out variable used to store matrials of the 3D model.
     * @return Whether the Load operation completed successfully.
     */
    bool load(const QString &objFilePath, QVector<Mesh> &meshes,
              QVector<Vertex> &vertices, QVector<unsigned short> &indices,
              QVector<Material> &materials);
private:
    /**
     * @brief Parses the .mtl file into materials container
     * @param fileName File path of .obj file.
     * @param materials Out variable used to store materials of the 3D model.
     * @return Whether the parsing operation completed successfully.
     */
    bool readMaterialLibrary(const QString &mtlFilePath, QVector<Material> &materials);
};

#endif // OBJLOADER_H
