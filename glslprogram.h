#ifndef GLSLPROGRAM_H
#define GLSLPROGRAM_H
#include <GL/glew.h>

#include <array>
#include <QString>
#include <QByteArray>
#include <QHash>
#include <QFile>

/**
 *@brief Provides necessary functions to build a program object
 * consisting of a vertex and fragment shader
 *
 * This class is used to:
 *
 * -# Build a GLSL Shader Program through linking and compiling vertex and fragment shaders.
 * -# Add attributes and uniforms to the program object.
 * -# Query attributes and uniforms location in the OpenGL context.
 * -# Enable and disable the program object for rendering.
 *
 * This is an abstract class. Specific GLSL programs are built by inheriting from this class
 * and using its protected functions. Sample usage:
 *
 * @code
 * loadShader(GL_VERTEX_SHADER, "shader.vert");
 * loadShader(GL_FRAGMENT_SHADER, "shader.frag");
 * CreateAndLinkProgram();
 * use();
 *    //Add attributes
 *    addAttribute("attribute1");
 *    addAttribute("attribute2");
 *
 *    //Add uniforms
 *    addUniform("uniform1");
 *    addUniform("uniform2");
 *
 *    //Specify the value of uniform variables for the current program object
 *    glUniform1i((*this)("uniform1"), 2);
 * unUse();
 * @endcode
 *
 */

class GLSLProgram {
public:
    /** @brief Default Constructor*/
    GLSLProgram();

    /** @brief Copying is not allowed*/
    GLSLProgram(const GLSLProgram&) = delete;

    /** @brief Copying is not allowed*/
    GLSLProgram& operator=(const GLSLProgram&) = delete;

    /** @brief Destructor */
    virtual ~GLSLProgram();

    /**
     * @brief Builds the Shader Program. Pure virtual function that must be defined in subclass
     */
    virtual void buildShaderProgram() = 0;

    /**
     * @brief Installs the program object as part of current rendering state
     *
     * Equivalent to:
     * @code
     * glUseProgram(programObject);
     * @endcode
     */
    void use() const;

    /**
     * @brief Removes the program object from the current rendering state
     *
     * Equivalent to:
     * @code
     * glUseProgram(0);
     * @endcode
     */
    void unUse() const;

    /**
     * @brief Deletes program object
     *
     * @code
     * glDeleteProgram(programObject);
     * @endcode
     */
    void deleteShaderProgram();

    /**
     * @param attributeName Name of attribute variable
     * @return Location of attribute variable in program object.
     */
    const GLuint operator[](const QString& attributeName) const;

    /**
     * @param uniformName Name of uniform variable
     * @return Location of uniform variable in program object.
     */
    const GLuint operator()(const QString& uniformName) const;

    /**
     * @param variableLocation Uniform or attribute location
     * @return Whether the attribute or uniform is active in the program object.
     */
    bool isActive(const GLuint variableLocation) const {
        return variableLocation != GLuint(-1);
    }

protected:
    /**
     * @brief Loads shader from text file
     * @param shaderType Type of shader (i.e. GL_VERTEX_SHADER or GL_FRAGMENT_SHADER).
     * @param shaderFileName File path of shader text file.
     */
    void loadShader(GLenum shaderType, const QString &shaderFileName);

    /**
     * @brief Creates and links program object from previously loaded vertex and fragment shaders
     */
    void createAndLinkProgram();

    /**
     * @brief Adds attribute variable
     * @param attributeName Name of attribute variable.
     */
    void addAttribute(const QString& attributeName);

    /**
     * @brief Adds uniform variable
     * @param uniformName Name of uniform variable.
     */
    void addUniform(const QString& uniformName);

private:
    /**
     * @brief Loads shader from text file
     * @param shaderFilename File path of shader text file.
     * @return String of text file contents (i.e. shader code) in QByteArray format.
     */
    QByteArray loadShaderFromFile(const QString &shaderFilename) const;

    /**
     * @brief Checks status after shader compiling or linking
     * @param objectID shaderID from glCreateShader() or programID from glCreateProgram().
     * @param glGetIVFunc glGetShaderiv for shaders or glGetProgramiv for program objects.
     * @param glGetInfoLogFunc glGetShaderInfoLog for shaders or glGetProgramInfoLog for program objects.
     * @param statusType GL_COMPILE_STATUS for shaders or GL_LINK_STATUS for program objects.
     * @return Whether compiling or linking completed successfully.
     */
    bool checkStatus(GLuint objectID, PFNGLGETSHADERIVPROC glGetIVFunc,
                     PFNGLGETSHADERINFOLOGPROC glGetInfoLogFunc, GLenum statusType) const;

    /**
     * @brief Checks shader status
     * @param shaderID shaderID from glCreateShader().
     * @return Whether compiling completed successfully.
     */
    bool checkShaderStatus(GLuint shaderID) const;

    /**
     * @brief Checks program status
     * @param programID programID from glCreateProgram().
     * @return Whether linking completed successfully.
     */
    bool checkProgramStatus(GLuint programID) const;

    enum ShaderType {VERTEX_SHADER_, FRAGMENT_SHADER_, TOTAL_SHADERS};

    std::array<GLuint, TOTAL_SHADERS> shaders_; //Index [0] : Vertex Shader, Index [1] : Fragment Shader

    QHash<QString, GLuint> attributeLocationList_;
    QHash<QString, GLuint> uniformLocationList_;

    GLuint	program_;   //is -1 if invalid
};

#endif // GLSLPROGRAM_H
