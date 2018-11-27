#ifndef GL_H
#define GL_H

#include <iostream>
#include <memory>
#include <fstream>
#include <sstream>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <SOIL/SOIL.h>
#include <glm/glm.hpp>

GLuint loadShaderFromSourceCode(GLenum type, const char* sourcecode, int length);
GLuint loadShaderFromFile(GLenum type, const char* filepath);

GLuint loadPlainTexture();
GLuint loadErrorTexture();
GLuint loadTextureFromData(glm::uvec2 size, const unsigned char* data);
GLuint loadTextureFromFile(const char* filepath);
GLuint loadTextureFromGenerator(glm::uvec2 size, glm::u8vec4(*generator)(glm::uvec2 pos, glm::uvec2 size));

#endif
