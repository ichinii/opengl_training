#include <iostream>
#include <cassert>
#include <chrono>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "OpenGlTraining.h"

void glfw_error_callback(int error, const char* description)
{
  std::cout << error << " " << description << std::endl;
}

int main()
{
  // init glfw
  glfwSetErrorCallback(glfw_error_callback);
  assert(glfwInit());

  // open window
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 5);
  auto* window = glfwCreateWindow(800, 600, "opengl_training", nullptr, nullptr);
  assert(window);

  glfwMakeContextCurrent(window);

  // init glew
  glewExperimental = GL_TRUE;
  {
    if (GLenum err = glewInit(); err != GLEW_OK)
    {
        /* Problem: glewInit failed, something is seriously wrong. */
        fprintf(stderr, "Error: %s\n", glewGetErrorString(err));

    }
  }

  // time management for main loop (max 9223372036854775808 milliseconds)
  using namespace std::chrono_literals;
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
