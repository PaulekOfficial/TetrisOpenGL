#include "structs.h"

#include <iostream>
Texture::Texture(GLuint iwidth, GLuint iheight, GLubyte* image, GLenum format) : width(iwidth), height(iheight)
{
	// Generate texture id
	glGenTextures(1, &id);
	glBindTexture(GL_TEXTURE_2D, id);

	// Load texture to ram
	glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, image);

	// Setup params
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
}


Shader::Shader(const GLchar* vertex, const GLchar* fragment)
{
	// load shader info
    GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vertex, NULL);
    glCompileShader(vertexShader);

    GLchar infoLog[512];
    GLint success;

	// compile shader, like in cod :cool:
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
    if (success == 0) {
        glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
        std::cout << "Error compiling vector shader: " << infoLog << std::endl;
    }

	// create fragment shader
    GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragment, NULL);
    glCompileShader(fragmentShader);

    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
    if (success == 0) {
        glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
        std::cout << "Error compiling fragment shader: " << infoLog << std::endl;
    }

	// create sub shader program
    GLuint program = glCreateProgram();
    glAttachShader(program, vertexShader);
    glAttachShader(program, fragmentShader);
    glLinkProgram(program);

    glGetProgramiv(program, GL_LINK_STATUS, &success);
    if (success == 0) {
        glGetProgramInfoLog(program, 512, NULL, infoLog);
        std::cout << "Error linking shader program: " << infoLog << std::endl;
    }

	// delete shader
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    id = program;
}
