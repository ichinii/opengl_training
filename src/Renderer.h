#ifndef OPENGL_TRAINING_RENDERER_H
#define OPENGL_TRAINING_RENDERER_H

#include <glm/glm.hpp>
#include <vector>
#include <array>
#include <GL/glew.h>

class Renderer {
public:
	Renderer();
	~Renderer();

	enum Texture {
		PlainTexture,
		TerrainTexture,
		PlayerTexture,
		NUM_TEXTURES
	};

	void clear();
	void flush();
	void setMvp(glm::mat4 value);
	void setTexture(Texture value);
	void drawTile(glm::vec2 position, glm::vec2 size, unsigned char tile);
	void drawPlayer(glm::vec2 position, glm::vec2 size, unsigned char tile);

private:
	GLint _program;
	GLuint _vao;
	GLuint _vbos[2];

	std::array<GLuint, NUM_TEXTURES> _textureIds;
	Texture _texture;
	glm::vec3 _color;

	std::vector<glm::vec2> _positions;
	std::vector<glm::vec2> _uvs;
	glm::mat4 _mvp;
};


#endif //OPENGL_TRAINING_RENDERER_H
