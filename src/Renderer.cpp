#include "Renderer.h"
#include <string>
#include <fstream>
#include <sstream>
#include <memory>
#include <iostream>
#include <SOIL/SOIL.h>

GLuint loadShaderFromSourceCode(GLenum type, const char* sourcecode, int length)
{
	GLuint shaderId = glCreateShader(type);

	glShaderSource(shaderId, 1, &sourcecode, &length);
	glCompileShader(shaderId);

	GLint isCompiled = 0;
	glGetShaderiv(shaderId, GL_COMPILE_STATUS, &isCompiled);
	if(isCompiled == GL_FALSE)
	{
		GLint maxLength = 0;
		glGetShaderiv(shaderId, GL_INFO_LOG_LENGTH, &maxLength);

		auto errorLog = std::make_unique<GLchar[]>(maxLength);
		glGetShaderInfoLog(shaderId, maxLength, &maxLength, &errorLog[0]);

		std::cout << "Error compiling " << std::endl
			<< &errorLog[0] << std::endl;
		glDeleteShader(shaderId); // Don't leak the shader.
		return 0;
	}

	return shaderId;
}

GLuint loadShaderFromFile(GLenum type, const char* filepath)
{
	std::ifstream fstream;
	fstream.open(filepath);

	if (!fstream.is_open())
	{
		std::cout << "Unable to open file '" << filepath << "'" << std::endl;
		return 0;
	}

	std::stringstream sstream;
	std::string line;
	while (std::getline(fstream, line))
		sstream << line << '\n';
	line = sstream.str();

	GLuint shaderId = loadShaderFromSourceCode(type, line.c_str(), line.length());
	if (!shaderId)
		std::cout << "...with filepath '" << filepath << "'"; 

	return shaderId;
} 

Renderer::Renderer() {
  // generate buffers
  glGenBuffers(2, vbos);
  glGenVertexArrays(1, &vao);
  glBindVertexArray(vao);

  // init position buffer
  glEnableVertexAttribArray(0);
  glBindBuffer(GL_ARRAY_BUFFER, vbos[0]);
  glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, nullptr);

  // init uv buffer
  glEnableVertexAttribArray(1);
  glBindBuffer(GL_ARRAY_BUFFER, vbos[1]);
  glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, nullptr);

  glBindVertexArray(0);

  GLuint vertexShader = loadShaderFromFile(GL_VERTEX_SHADER, "shader/vertex.glsl");
  GLuint fragmentShader = loadShaderFromFile(GL_FRAGMENT_SHADER, "shader/fragment.glsl");
  program = glCreateProgram();
  glAttachShader(program, vertexShader);
  glAttachShader(program, fragmentShader);
  glLinkProgram(program);

  glDeleteShader(vertexShader);
  glDeleteShader(fragmentShader);

  mvp = glm::mat4(1.f);
  positions.reserve(1024);
  uvs.reserve(1024);

  int width, height, channels;
  unsigned char *ht_map = SOIL_load_image (
    "terrain.png",
    &width, &height, &channels,
    SOIL_LOAD_RGBA
  );

  glGenTextures(1, &texture);
  glBindTexture(GL_TEXTURE_2D, texture);
  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, ht_map);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
  delete[] ht_map;
}

Renderer::~Renderer() {
  glDeleteProgram(program);
  glDeleteVertexArrays(1, &vao);
  glDeleteBuffers(2, vbos);
}

void Renderer::drawTile(glm::vec2 position, glm::vec2 size, unsigned char tile) {
  if (positions.size() + 6 >= positions.capacity())
    flush();

  positions.push_back(position);
  positions.push_back(position + glm::vec2(size.x, 0));
  positions.push_back(position + glm::vec2(size.x, size.y));

  positions.push_back(position);
  positions.push_back(position + glm::vec2(size.x, size.y));
  positions.push_back(position + glm::vec2(0, size.y));

  glm::vec2 uv = glm::vec2(tile % 16, tile / 16);
  glm::vec2 uvSize(16, 16);
  uvs.push_back(uv / uvSize);
  uvs.push_back((uv + glm::vec2(1, 0)) / uvSize);
  uvs.push_back((uv + glm::vec2(1, 1)) / uvSize);

  uvs.push_back(uv / uvSize);
  uvs.push_back((uv + glm::vec2(1, 1)) / uvSize);
  uvs.push_back((uv + glm::vec2(0, 1)) / uvSize);
}

void Renderer::setMvp(glm::mat4 mvp) {
  if (this->mvp != mvp) {
    this->mvp = mvp;
    flush();
  }
}

void Renderer::clear() {
  positions.clear();
  uvs.clear();
}

void Renderer::flush() {
  assert(positions.size() == uvs.size());

  if (positions.size() == 0)
    return;

  glUseProgram(program);

  auto mvpLocation = glGetUniformLocation(program, "mvp");
  glUniformMatrix4fv(mvpLocation, 1, GL_FALSE, &mvp[0][0]);

  glBindBuffer(GL_ARRAY_BUFFER, vbos[0]);
  glBufferData(GL_ARRAY_BUFFER, positions.size() * sizeof(glm::vec2), positions.data(), GL_STATIC_DRAW);

  glBindBuffer(GL_ARRAY_BUFFER, vbos[1]);
  glBufferData(GL_ARRAY_BUFFER, uvs.size() * sizeof(glm::vec2), uvs.data(), GL_STATIC_DRAW);

  glBindVertexArray(vao);
  glDrawArrays(GL_TRIANGLES, 0, positions.size());
  glBindVertexArray(0);

  clear();
}
