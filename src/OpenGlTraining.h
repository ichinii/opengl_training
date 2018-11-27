#ifndef OPENGL_TRAINING_H
#define OPENGL_TRAINING_H

#include <chrono>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "Renderer.h"
#include "PlayerController.h"

class OpenGlTraining {
public:
	void init(GLFWwindow* window);
	void update(std::chrono::milliseconds deltaTime);
	void render();

private:
	Renderer renderer;
	GLFWwindow *window;
	std::vector<unsigned char> world;
	PlayerController playerController;
	glm::vec2 playerPosition = glm::vec2(7.5f, 7.5f);

	void drawWorld();
	void drawPlayer();
	void loadWorld();
};

#endif
