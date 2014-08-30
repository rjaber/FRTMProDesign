#include "glslprogram.h"

#include <memory>
#include <QFileDevice>
#include <QFileInfo>
#include <QTextStream>
#include <QDebug>

#include "frtmutils.h"


GLSLProgram::GLSLProgram() :
    program_(-1),
    shaders_({0}){
}

GLSLProgram::~GLSLProgram(){
    deleteShaderProgram();
}

void GLSLProgram::use() const {
    if (program_ != -1)
        glUseProgram(program_);
}

void GLSLProgram::unUse() const {
    glUseProgram(0);
}

void GLSLProgram::deleteShaderProgram() {
    attributeLocationList_.clear();
    uniformLocationList_.clear();

    if (program_ != -1 ) {
        glDeleteProgram(program_);
        shaders_.fill(0);
        program_ = -1;
    }
}

const GLuint GLSLProgram::operator[](const QString& attributeName) const {
     if (attributeLocationList_.contains(attributeName)){
         return attributeLocationList_[attributeName];
     }
     else {
         qDebug() << "Attribute does not exist.";
         return GLuint(-1);
     }
}

const GLuint GLSLProgram::operator()(const QString& uniformName) const {
    if (uniformLocationList_.contains(uniformName)){
        return uniformLocationList_[uniformName];
    }
    else {
        qDebug() << "Uniform does not exist.";
        return GLuint(-1);
    }
}

void GLSLProgram::loadShader(GLenum shaderType, const QString &shaderFileName) {
    GLuint shader = glCreateShader(shaderType);

    QByteArray shaderSource = loadShaderFromFile(shaderFileName);

    //glShaderSource()'s 3rd arg takes (const GLchar * const *c)
    const GLchar *shaderSourceAdapter[1] = {shaderSource.data()};
    glShaderSource(shader, 1, shaderSourceAdapter , 0);

    glCompileShader(shader);

    if (!checkShaderStatus(shader)) {
        qDebug() << "^^Shader Compile Error^^";
        return;
    }

    if (shaderType == GL_VERTEX_SHADER)
        shaders_[VERTEX_SHADER_] = shader;
    else if (shaderType == GL_FRAGMENT_SHADER)
        shaders_[FRAGMENT_SHADER_] = shader;
}

void GLSLProgram::createAndLinkProgram() {
    program_ = glCreateProgram();

    if (shaders_[VERTEX_SHADER_] != 0)
        glAttachShader (program_, shaders_[VERTEX_SHADER_]);

    if (shaders_[FRAGMENT_SHADER_] != 0)
        glAttachShader (program_, shaders_[FRAGMENT_SHADER_]);

    glLinkProgram (program_);

    if ( !checkProgramStatus(program_) ) {
        qDebug() << "^^GLSL Program Error^^";
        return;
    }

    //No longer needed, shader contents have been copied into program object
    glDetachShader(program_, shaders_[VERTEX_SHADER_]);
    glDetachShader(program_, shaders_[FRAGMENT_SHADER_]);
    glDeleteShader(shaders_[VERTEX_SHADER_]);
    glDeleteShader(shaders_[FRAGMENT_SHADER_]);
}

void GLSLProgram::addAttribute(const QString &attributeName) {
    QByteArray tmp = attributeName.toLatin1();
    const char *attributeNameCString = tmp.data();
    attributeLocationList_[attributeName] = glGetAttribLocation(program_,
                                                                attributeNameCString);
}

void GLSLProgram::addUniform(const QString &uniformName) {
    QByteArray tmp = uniformName.toLatin1();
    const char *uniformNameCString = tmp.data();
    uniformLocationList_[uniformName] = glGetUniformLocation(program_,
                                                             uniformNameCString);
}

QByteArray GLSLProgram::loadShaderFromFile(const QString &shaderFilename) const {
    QFile shaderFile(shaderFilename);

    if (!shaderFile.open(QIODevice::ReadOnly))
        FRTMUtils::readFileErrorInfo(shaderFile);

    QString shaderCode;
    QTextStream iStream(&shaderFile);
    while (!iStream.atEnd()) {
        shaderCode = iStream.readAll();
    }

    shaderFile.close();
    return shaderCode.toLatin1();
}

bool GLSLProgram::checkStatus(GLuint objectID,
                              PFNGLGETSHADERIVPROC glGetIVFunc,
                              PFNGLGETSHADERINFOLOGPROC glGetInfoLogFunc,
                              GLenum statusType) const {

        GLint status;
        glGetIVFunc(objectID, statusType, &status);
        if (status != GL_TRUE) {
            GLint infoLogLength;
            glGetIVFunc(objectID, GL_INFO_LOG_LENGTH, &infoLogLength);

            GLsizei infoLogBufferSize;
            std::unique_ptr<GLchar[]> infoLogBuffer(new GLchar[infoLogLength]);

            glGetInfoLogFunc(objectID, infoLogLength, &infoLogBufferSize, infoLogBuffer.get());

            QString logMessage = QString::fromLatin1(static_cast<char*>(infoLogBuffer.get()));
            qDebug() << logMessage;

            return false;
        }
    return true;
}

bool GLSLProgram::checkShaderStatus(GLuint shaderID) const {
    return checkStatus(shaderID, glGetShaderiv, glGetShaderInfoLog, GL_COMPILE_STATUS);
}

bool GLSLProgram::checkProgramStatus(GLuint programID) const {
    return checkStatus(programID, glGetProgramiv, glGetProgramInfoLog, GL_LINK_STATUS);
}
