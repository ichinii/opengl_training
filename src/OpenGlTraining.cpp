#include "OpenGlTraining.h"

void OpenGlTraining::init(GLFWwindow* window)
{
  
}

void OpenGlTraining::update(std::chrono::milliseconds deltaTime)
{

}

void OpenGlTraining::render()
{
  renderer.drawTile(glm::vec2(), glm::vec2(1, 1), 0);
  renderer.flush();
}
