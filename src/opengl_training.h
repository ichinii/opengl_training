#ifndef OPENGL_TRAINING_H
#define OPENGL_TRAINING_H

#include <chrono>
#include <GL/glew.h>
#include <GLFW/glfw3.h>

class OpenGlTraining {
public:
  static void init(GLFWwindow* window);
  static void update(std::chrono::milliseconds deltaTime);
  static void render();
};

#endif
