cmake_minimum_required(VERSION 2.8)
project(opengl_training)

set(SOURCES
  src/main.cpp
  src/opengl_training.cpp)

set(HEADERS
  src/opengl_training.h)

set(CXX_FLAGS
  --std=c++17)

#set(LINK_FLAGS)

set(CMAKE_CXX_FLAGS ${CXX_FLAGS})
#set(CMAKE_CXX_FLAGS ${LINK_FLAGS})
add_executable(opengl_training ${SOURCES} ${HEADERS})
target_include_directories(opengl_training INTERFACE "src")
target_link_libraries(opengl_training PUBLIC GL GLU GLEW glfw)
