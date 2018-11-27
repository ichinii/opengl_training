#include "Player.h"

Player::Player() {
    position = glm::vec2(7.5, 7.5);
}

void Player::update(std::chrono::milliseconds deltaTime) {
    controller.update(deltaTime);

    auto direction = controller.direction();
    position += direction * (speed * deltaTime.count() / 1000.f);
}

void Player::setInputController(InputController controller) {
    Player::controller = controller;
}
