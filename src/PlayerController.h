#ifndef PLAYER_CONTROLLER_H
#define PLAYER_CONTROLLER_H

#include <chrono>
#include "gl.h"

class PlayerController {
public:
	PlayerController();

	void update(std::chrono::milliseconds deltaTile);
	void setWindow(GLFWwindow* window);

private:
	GLFWwindow* _window;
};

#endif
