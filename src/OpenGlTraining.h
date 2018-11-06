#ifndef OPENGL_TRAINING_H
#define OPENGL_TRAINING_H

#include <chrono>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "Renderer.h"
#include <memory>

class OpenGlTraining {
  Renderer renderer;

public:
  void init(GLFWwindow* window);
  void update(std::chrono::milliseconds deltaTime);
  void render();
};

#endif
