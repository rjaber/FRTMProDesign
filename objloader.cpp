#include "objloader.h"

#include <algorithm>
#include <QFileDevice>
#include <QTextStream>
#include <QDebug>

bool ObjLoader::load(const QString &objFilePath, QVector<Mesh> &meshes,
          QVector<Vertex> &vertices, QVector<unsigned short> &indices,
          QVector<Material> &materials) {

    QFile objFile(objFilePath);

    if (!objFile.open(QIODevice::ReadOnly))
        readFileErrorInfo(objFile);

    QVector<glm::vec3> unsortedVertices;
    QVector<glm::vec3> unsortedNormals;
    QVector<glm::vec2> unsortedUVs;
    int totalTriangles = 0;

    QTextStream iStream(&objFile);
    QString currLine;

    while (!iStream.atEnd()) {
        currLine = iStream.readLine();

        if (currLine.size() == 0 || currLine.leftRef(1) == "#")      //skip line if blank or comment
            continue;

        QStringList currLineStringList = currLine.split(" ", QString::SkipEmptyParts);
        QString prefix = currLineStringList[0];

        if (prefix == "v") {
            unsortedVertices.push_back(glm::vec3());
            unsortedVertices.back().x = currLineStringList[1].toFloat();
            unsortedVertices.back().y = currLineStringList[2].toFloat();
            unsortedVertices.back().z = currLineStringList[3].toFloat();
        }
        else if (prefix == "vt") {
            unsortedUVs.push_back(glm::vec2());
            unsortedUVs.back().x = currLineStringList[1].toFloat();
            unsortedUVs.back().y = currLineStringList[2].toFloat();
        }
        else if (prefix == "vn") {
            unsortedNormals.push_back(glm::vec3());
            unsortedNormals.back().x = currLineStringList[1].toFloat();
            unsortedNormals.back().y = currLineStringList[2].toFloat();
            unsortedNormals.back().z = currLineStringList[3].toFloat();
        }
        else if (prefix == "f") {
            Face face;

            QStringList faceVertixAttributes = currLineStringList[1].split('/');
            face.v1 = faceVertixAttributes[0].toUShort() - 1;
            face.vt1 = faceVertixAttributes[1].toUShort() - 1;
            face.vn1 = faceVertixAttributes[2].toUShort() - 1;

            faceVertixAttributes = currLineStringList[2].split('/');
            face.v2 = faceVertixAttributes[0].toUShort() - 1;
            face.vt2 = faceVertixAttributes[1].toUShort() - 1;
            face.vn2 = faceVertixAttributes[2].toUShort() - 1;

            faceVertixAttributes = currLineStringList[3].split('/');
            face.v3 = faceVertixAttributes[0].toUShort() - 1;
            face.vt3 = faceVertixAttributes[1].toUShort() - 1;
            face.vn3 = faceVertixAttributes[2].toUShort() - 1;

            ++totalTriangles;
            //store indices of current mesh in the material associated with it
            if (meshes.back().materialIndex != -1) {
                materials[meshes.back().materialIndex].subIndices.push_back(face.v1);
                materials[meshes.back().materialIndex].subIndices.push_back(face.vn1);
                materials[meshes.back().materialIndex].subIndices.push_back(face.vt1);
                materials[meshes.back().materialIndex].subIndices.push_back(face.v2);
                materials[meshes.back().materialIndex].subIndices.push_back(face.vn2);
                materials[meshes.back().materialIndex].subIndices.push_back(face.vt2);
                materials[meshes.back().materialIndex].subIndices.push_back(face.v3);
                materials[meshes.back().materialIndex].subIndices.push_back(face.vn3);
                materials[meshes.back().materialIndex].subIndices.push_back(face.vt3);
            }
        }
        else if (prefix == "mtllib") {
            readMaterialLibrary(getModelFilePath(objFilePath) + currLineStringList[1], materials);
        }
        else if (prefix == "usemtl") {
            QString materialName = currLineStringList[1];
            int materialIndex = -1;
            //find material index for current mesh
            auto itrMaterial = std::find_if(materials.begin(), materials.end(),
                                    [&materialName, &materialIndex] (const Material& m) {
                                            ++materialIndex;
                                            return m.name == materialName;} );

            meshes.back().materialIndex = itrMaterial != materials.end() ? materialIndex : -1;
        }
        else if (prefix == "g") {
            meshes.push_back(Mesh());
            meshes.back().name = currLineStringList[1];
        }
    }

    vertices.resize(totalTriangles * 3);

    int countVertices = 0;
    int countIndices = 0;
    int countSubIndices = 0;

    //sort vertix attributes so that they can be accessed sequentially when loaded into VBOs
    for (size_t i=0; i<materials.size(); ++i) {

        materials[i].offset = countVertices;

        for (size_t j = 0; j < materials[i].subIndices.size(); ) {
            vertices[countVertices].pos = unsortedVertices[materials[i].subIndices[j++]];
            vertices[countVertices].normal = unsortedNormals[materials[i].subIndices[j++]];
            vertices[countVertices++].uv = unsortedUVs[materials[i].subIndices[j++]];

            indices.push_back(countIndices++);
            countSubIndices++;
        }

        materials[i].count = countSubIndices;
        countSubIndices = 0;
    }

    objFile.close();

    return true;
}

bool ObjLoader::readMaterialLibrary(const QString &mtlFilePath, QVector<Material> &materials) {

    QFile mtlFile(mtlFilePath);

    if (!mtlFile.open(QIODevice::ReadOnly))
        readFileErrorInfo(mtlFile);

    QTextStream iStream(&mtlFile);
    QString currLine;

    while (!iStream.atEnd()) {
        currLine = iStream.readLine();

        if (currLine.size() == 0 || currLine.leftRef(1) == "#")     //skip line if blank or comment
            continue;

        QStringList currLineStringList = currLine.split(" ", QString::SkipEmptyParts);
        QString prefix = currLineStringList[0];

        if (prefix.left(1) == QString('\t'))    //if first char is a tab remove from string list
           prefix.remove(0, 1);

        if (prefix == "newmtl") {
            materials.push_back(Material());
            materials.back().name = currLineStringList[1];
        }
        else if (prefix == "Ns") {
            materials.back().Ns = currLineStringList[1].toFloat();
        }
        else if (prefix == "Ni") {
            materials.back().Ni = currLineStringList[1].toFloat();
        }
        else if (prefix == "d") {
            materials.back().d = currLineStringList[1].toFloat();
        }
        else if (prefix == "Tr") {
            materials.back().Tr = currLineStringList[1].toFloat();
        }
        else if (prefix == "Tf") {
            materials.back().Tf.x = currLineStringList[1].toFloat();
            materials.back().Tf.y = currLineStringList[2].toFloat();
            materials.back().Tf.z = currLineStringList[3].toFloat();
        }
        else if (prefix == "illum") {
            materials.back().illum = currLineStringList[1].toFloat();
        }
        else if (prefix == "Ka") {
            materials.back().Ka.x = currLineStringList[1].toFloat();
            materials.back().Ka.y = currLineStringList[2].toFloat();
            materials.back().Ka.z = currLineStringList[3].toFloat();
        }
        else if (prefix == "Kd") {
            materials.back().Kd.x = currLineStringList[1].toFloat();
            materials.back().Kd.y = currLineStringList[2].toFloat();
            materials.back().Kd.z = currLineStringList[3].toFloat();
        }
        else if (prefix == "Ks") {
            materials.back().Ks.x = currLineStringList[1].toFloat();
            materials.back().Ks.y = currLineStringList[2].toFloat();
            materials.back().Ks.z = currLineStringList[3].toFloat();
        }
        else if (prefix == "Ke") {
            materials.back().Ke.x = currLineStringList[1].toFloat();
            materials.back().Ke.y = currLineStringList[2].toFloat();
            materials.back().Ke.z = currLineStringList[3].toFloat();
        }
        else if (prefix == "map_Ka") {
            materials.back().map_Ka = currLineStringList[1];
        }
        else if (prefix == "map_Kd") {
            materials.back().map_Kd = currLineStringList[1];
        }
    }

    mtlFile.close();

    return true;
}

QString ObjLoader::getModelFilePath(const QString &objFilePath) {
    QStringList modelFilePathStringList = objFilePath.split( "/", QString::SkipEmptyParts );
    modelFilePathStringList.removeLast();

    QString modelFilePath = modelFilePathStringList.join( "/" );
    modelFilePath.append("/");
    return modelFilePath;
}

void ObjLoader::readFileErrorInfo(const QFile &file) {
        QString errMsg;
        errMsg = file.errorString();

        QFileDevice::FileError errStatus = QFileDevice::NoError;
        errStatus = file.error();

        qDebug () <<"Error Opening File" << "\nError Message: " << errMsg << "\nError Status:" << errStatus ;
}

