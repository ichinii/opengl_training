#include "OpenGlTraining.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

void OpenGlTraining::init(GLFWwindow* window)
{
  this->window = window;
}

void OpenGlTraining::update(std::chrono::milliseconds deltaTime)
{

}

void OpenGlTraining::render()
{
  glm::mat4 m, v, p;
  m = glm::mat4(1.0);
  v = glm::mat4(1.0);

  int width, height;
  glfwGetWindowSize(window, &width, &height);

  p = glm::ortho<float>(0, 16.f * width / height, 0, 16);

  renderer.setMvp(p * v * m);

  renderer.drawTile(glm::vec2(), glm::vec2(1, 1), 0);
  renderer.drawTile(glm::vec2(0,1), glm::vec2(1, 1), 1);
  renderer.flush();
}
