#include "PlayerController.h"

PlayerController::PlayerController()
	: _window(nullptr)
{
}

void PlayerController::update(std::chrono::milliseconds deltaTime)
{
	assert(_window);

	auto left = glfwGetKey(_window, GLFW_KEY_A);
	auto right = glfwGetKey(_window, GLFW_KEY_D);
	auto up = glfwGetKey(_window, GLFW_KEY_W);
	auto down = glfwGetKey(_window, GLFW_KEY_S);
}

void PlayerController::setWindow(GLFWwindow* window)
{
	_window = window;
}
