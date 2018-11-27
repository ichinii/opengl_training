#ifndef OPENGL_TRAINING_PLAYER_H
#define OPENGL_TRAINING_PLAYER_H


#include <glm/vec2.hpp>
#include <chrono>
#include "InputController.h"

class Player {
public:
    glm::vec2 position;
    float speed = 10.f;

    Player();
    void update(std::chrono::milliseconds deltaTime);
    void setInputController(InputController controller);

private:
    InputController controller;
};


#endif //OPENGL_TRAINING_PLAYER_H
