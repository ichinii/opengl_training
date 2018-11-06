#include <iostream>
#include <cassert>
#include <chrono>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "opengl_training.h"

using namespace std::chrono_literals;


int main()
{
  // init glfw
  assert(glfwInit());

  // open window
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 5);
  auto* window = glfwCreateWindow(800, 600, "opengl_training", nullptr, nullptr);
  assert(window);

  glfwMakeContextCurrent(window);

  // init glew
  assert(!glewInit());

  // time management for main loop (max 9223372036854775808 milliseconds)
  using clock = std::chrono::steady_clock;
  auto startTime = clock::now();
  auto totalDeltaTime = 0ms;

  // main loop
  OpenGlTraining openglTraining;
  openglTraining.init(window);
  while (!glfwWindowShouldClose(window)) {
    auto curTime = clock::now();
    auto deltaTime = std::chrono::duration_cast<std::chrono::milliseconds>(clock::now() - (startTime + totalDeltaTime));
    totalDeltaTime += deltaTime;

    // update
    openglTraining.update(deltaTime);

    // render
    glClear(GL_COLOR_BUFFER_BIT);
    openglTraining.render();
    glfwSwapBuffers(window);

    glfwPollEvents();
    if (glfwGetKey(window, GLFW_KEY_Q))
      glfwSetWindowShouldClose(window, true);
  }

  // terminate glfw
  glfwTerminate();
  return 0;
}
