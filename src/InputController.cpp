#include "InputController.h"

InputController::InputController()
{
}

void InputController::update(std::chrono::milliseconds deltaTime)
{
	assert(_window);

	auto left = glfwGetKey(_window, GLFW_KEY_A) == GLFW_PRESS;
	auto right = glfwGetKey(_window, GLFW_KEY_D) == GLFW_PRESS;
	auto up = glfwGetKey(_window, GLFW_KEY_W) == GLFW_PRESS;
	auto down = glfwGetKey(_window, GLFW_KEY_S) == GLFW_PRESS;

	dir = glm::vec2(0, 0);

	if (up)
		dir += glm::vec2(0, 1);
	else if (down)
		dir += glm::vec2(0, -1);
	if (left)
		dir += glm::vec2(-1, 0);
	else if (right)
		dir += glm::vec2(1, 0);

	if (glm::length(dir) != 0)
		dir = glm::normalize(dir);
}

void InputController::setWindow(GLFWwindow* window)
{
	_window = window;
}

glm::vec2 InputController::direction() {
	return dir;
}
