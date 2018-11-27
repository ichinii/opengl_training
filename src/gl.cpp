#include "gl.h"

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
