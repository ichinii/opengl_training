#include "Renderer.h"
#include "gl.h"

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
	_textureIds[TerrainTexture] = loadTextureFromFile("res/terrain.png");
	_textureIds[PlayerTexture] = loadTextureFromFile("res/skeleton.png");
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
	setTexture(TerrainTexture);

	if (_positions.size() + 6 >= _positions.capacity())
		flush();

	_positions.push_back(position);
	_positions.push_back(position + glm::vec2(size.x, 0));
	_positions.push_back(position + glm::vec2(size.x, size.y));

	_positions.push_back(position);
	_positions.push_back(position + glm::vec2(size.x, size.y));
	_positions.push_back(position + glm::vec2(0, size.y));

	glm::vec2 uv = glm::vec2(tile % 16, 15 - tile / 16);
	glm::vec2 uvSize(16, 16);
	_uvs.push_back(uv / uvSize);
	_uvs.push_back((uv + glm::vec2(1, 0)) / uvSize);
	_uvs.push_back((uv + glm::vec2(1, 1)) / uvSize);

	_uvs.push_back(uv / uvSize);
	_uvs.push_back((uv + glm::vec2(1, 1)) / uvSize);
	_uvs.push_back((uv + glm::vec2(0, 1)) / uvSize);
}

void Renderer::drawPlayer(glm::vec2 position, glm::vec2 size, unsigned char tile)
{
	setTexture(PlayerTexture);

	if (_positions.size() + 6 >= _positions.capacity())
		flush();

	_positions.push_back(position);
	_positions.push_back(position + glm::vec2(size.x, 0));
	_positions.push_back(position + glm::vec2(size.x, size.y));

	_positions.push_back(position);
	_positions.push_back(position + glm::vec2(size.x, size.y));
	_positions.push_back(position + glm::vec2(0, size.y));

	glm::vec2 uv = glm::vec2(tile % 9, 3 - tile / 4);
	glm::vec2 uvSize(9, 4);
	_uvs.push_back(uv / uvSize);
	_uvs.push_back((uv + glm::vec2(1, 0)) / uvSize);
	_uvs.push_back((uv + glm::vec2(1, 1)) / uvSize);

	_uvs.push_back(uv / uvSize);
	_uvs.push_back((uv + glm::vec2(1, 1)) / uvSize);
	_uvs.push_back((uv + glm::vec2(0, 1)) / uvSize);
}

