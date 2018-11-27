#ifndef PLAYER_CONTROLLER_H
#define PLAYER_CONTROLLER_H

#include <chrono>
#include "gl.h"

class PlayerController {
public:
	PlayerController();

	void update(std::chrono::milliseconds deltaTile);
	void setWindow(GLFWwindow* window);

	glm::vec2 direction();

private:
	glm::vec2 dir = glm::vec2(0,0);
	GLFWwindow* _window = nullptr;
};

#endif
