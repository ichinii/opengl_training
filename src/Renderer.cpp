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

GLuint loadTextureFromData(glm::uvec2 size, const unsigned char* data)
{
	GLuint textureId;

	glGenTextures(1, &textureId);
	glBindTexture(GL_TEXTURE_2D, textureId);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, size.x, size.y, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

	return textureId;
}

GLuint loadTextureFromGenerator(glm::uvec2 size, glm::u8vec4(*generator)(glm::uvec2 pos, glm::uvec2 size))
{
	glm::u8vec4 data[size.x * size.y];
	for (std::size_t x = 0; x < size.x; x++)
		for (std::size_t y = 0; y < size.x; y++)
		{
			auto color = generator(glm::vec2(x, y), size);
			data[y * size.x + x] = color;
		}

	return loadTextureFromData(size, reinterpret_cast<unsigned char*>(&data));
}

GLuint loadPlainTexture()
{
	return loadTextureFromGenerator(glm::ivec2(16, 16), [] (auto, auto) -> glm::u8vec4 { return {255, 255, 255, 255}; });
}

GLuint loadErrorTexture()
{
	return loadTextureFromGenerator(glm::uvec2(256, 256), [] (glm::uvec2 pos, glm::uvec2 size) -> glm::u8vec4 {
		float x = pos.x / size.x * 3.14f;
		float y = pos.y / size.y * 3.14f;
		return glm::vec4(
			std::sin(x),
			std::cos(y),
			1,
			1) * 255.f;
	});
}

GLuint loadTextureFromFile(const char* filepath)
{
	GLuint textureId;

	int width, height, channels;
	unsigned char* data = SOIL_load_image(
		filepath,
		&width, &height, &channels,
		SOIL_LOAD_RGBA
	);

	if (data) {
		assert(channels == 4);
		textureId = loadTextureFromData(glm::uvec2(width, height), data);
	} else {
		std::cout << "unable to load texture '" << filepath << "'" << std::endl
			<< SOIL_last_result() << std::endl;

		textureId = loadErrorTexture();
	}

	delete[] data;
	return textureId;
}

Renderer::Renderer()
{
	// generate buffers
	glGenBuffers(2, _vbos);
	glGenVertexArrays(1, &_vao);
	glBindVertexArray(_vao);

	// init position buffer
	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, _vbos[0]);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, nullptr);

	// init uv buffer
	glEnableVertexAttribArray(1);
	glBindBuffer(GL_ARRAY_BUFFER, _vbos[1]);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, nullptr);

	glBindVertexArray(0);

	GLuint vertexShader = loadShaderFromFile(GL_VERTEX_SHADER, "shader/vertex.glsl");
	GLuint fragmentShader = loadShaderFromFile(GL_FRAGMENT_SHADER, "shader/fragment.glsl");
	_program = glCreateProgram();
	glAttachShader(_program, vertexShader);
	glAttachShader(_program, fragmentShader);
	glLinkProgram(_program);
	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);

	_mvp = glm::mat4(1.f);
	_positions.reserve(1024);
	_uvs.reserve(1024);

	_texture = PlainTexture;
	_textureIds[PlainTexture] = loadPlainTexture();
	_textureIds[TerrainTexture] = loadTextureFromFile("terrain.png");
	glBindTexture(GL_TEXTURE_2D, _textureIds[_texture]);

	setTexture(TerrainTexture);
}

Renderer::~Renderer()
{
	glDeleteProgram(_program);
	glDeleteVertexArrays(1, &_vao);
	glDeleteBuffers(2, _vbos);
}

void Renderer::clear()
{
	_positions.clear();
	_uvs.clear();
}

void Renderer::flush()
{
	assert(_positions.size() == _uvs.size());

	if (_positions.size() == 0)
		return;

	glUseProgram(_program);

	auto mvpLocation = glGetUniformLocation(_program, "mvp");
	glUniformMatrix4fv(mvpLocation, 1, GL_FALSE, &_mvp[0][0]);

	glBindBuffer(GL_ARRAY_BUFFER, _vbos[0]);
	glBufferData(GL_ARRAY_BUFFER, _positions.size() * sizeof(glm::vec2), _positions.data(), GL_STATIC_DRAW);

	glBindBuffer(GL_ARRAY_BUFFER, _vbos[1]);
	glBufferData(GL_ARRAY_BUFFER, _uvs.size() * sizeof(glm::vec2), _uvs.data(), GL_STATIC_DRAW);

	glBindVertexArray(_vao);
	glDrawArrays(GL_TRIANGLES, 0, _positions.size());
	glBindVertexArray(0);

	clear();
}

void Renderer::setMvp(glm::mat4 value)
{
	if (_mvp != value) {
		flush();
		_mvp = value;
	}
}

void Renderer::setTexture(Texture value)
{
	if (_texture != value) {
		flush();
		_texture = value;
		glBindTexture(GL_TEXTURE_2D, _textureIds[value]);
	}
}

void Renderer::drawTile(glm::vec2 position, glm::vec2 size, unsigned char tile)
{
	if (_positions.size() + 6 >= _positions.capacity())
		flush();

	_positions.push_back(position);
	_positions.push_back(position + glm::vec2(size.x, 0));
	_positions.push_back(position + glm::vec2(size.x, size.y));

	_positions.push_back(position);
	_positions.push_back(position + glm::vec2(size.x, size.y));
	_positions.push_back(position + glm::vec2(0, size.y));

	glm::vec2 uv = glm::vec2(tile % 16, tile / 16);
	glm::vec2 uvSize(16, 16);
	_uvs.push_back(uv / uvSize);
	_uvs.push_back((uv + glm::vec2(1, 0)) / uvSize);
	_uvs.push_back((uv + glm::vec2(1, 1)) / uvSize);

	_uvs.push_back(uv / uvSize);
	_uvs.push_back((uv + glm::vec2(1, 1)) / uvSize);
	_uvs.push_back((uv + glm::vec2(0, 1)) / uvSize);
}
