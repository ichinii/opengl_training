cmake_minimum_required(VERSION 3.12)
project(opengl_training)

set(SOURCES
  src/main.cpp
  src/OpenGlTraining.cpp
	src/Renderer.cpp
	src/gl.cpp
	src/PlayerController.cpp)

set(HEADERS
  src/OpenGlTraining.h
	src/Renderer.h
	src/gl.h
	src/PlayerController.h)

set(CXX_FLAGS
  --std=c++17)

#set(LINK_FLAGS)

set(CMAKE_CXX_FLAGS ${CXX_FLAGS})
#set(CMAKE_CXX_FLAGS ${LINK_FLAGS})
add_executable(opengl_training ${SOURCES} ${HEADERS})
target_include_directories(opengl_training INTERFACE "src")
target_link_libraries(opengl_training PUBLIC GL GLU GLEW glfw SOIL)

configure_file(res/sample.wld ${CMAKE_CURRENT_BINARY_DIR}/res/sample.wld COPYONLY)
configure_file(res/terrain.png ${CMAKE_CURRENT_BINARY_DIR}/res/terrain.png COPYONLY)
configure_file(res/skeleton.png ${CMAKE_CURRENT_BINARY_DIR}/res/skeleton.png COPYONLY)
configure_file(${CMAKE_CURRENT_SOURCE_DIR}/shader/vertex.glsl ${CMAKE_CURRENT_BINARY_DIR}/shader/vertex.glsl COPYONLY)
configure_file(${CMAKE_CURRENT_SOURCE_DIR}/shader/fragment.glsl ${CMAKE_CURRENT_BINARY_DIR}/shader/fragment.glsl COPYONLY)
