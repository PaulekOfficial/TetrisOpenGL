#if defined(__APPLE__)
#include <OpenGL/gl3.h>
#else
#define GLEW_STATIC
#include <GL/glew.h>
#endif
#include "glm/glm.hpp"
#include <glm/gtc/type_ptr.hpp>

class Texture
{
public:
	const GLuint height;
	const GLuint width;

    Texture() : width(0), height(0) {};

	Texture(GLuint iwidth, GLuint iheight, GLubyte* image, GLenum format);

	void bind() const { glBindTexture(GL_TEXTURE_2D, id); }
private:
	GLuint id;
};

class Shader
{
public:
    // form https://github.com/datenwolf/codesamples/blob/master/samples/OpenGL/minimal_glsl/minimal_glsl.c
    Shader(const GLchar* vertex, const GLchar* fragment);

    void setFloat(const GLchar* name, GLfloat value) const {
        glUniform1f(glGetUniformLocation(id, name), value);
    }

    void setMat4(const GLchar* name, const glm::mat4& matrix) const {
        glUniformMatrix4fv(glGetUniformLocation(id, name), 1, GL_FALSE, glm::value_ptr(matrix));
    }

    void setVec3(const GLchar* name, glm::vec3 vec) const {
        glUniform3f(glGetUniformLocation(id, name), vec.x, vec.y, vec.z);
    }

    void setVec2(const GLchar* name, glm::vec2 vec) const {
        glUniform2f(glGetUniformLocation(id, name), vec.x, vec.y);
    }

    void use() const { glUseProgram(id); }

private:
    GLuint id;
};


struct FragmentGlyph
{
    Texture texture;
    glm::ivec2 bearing;
    GLint64 advance;
};
