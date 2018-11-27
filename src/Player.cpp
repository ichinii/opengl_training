#include "Player.h"

Player::Player() {
    position = glm::vec2(7.5, 7.5);
}

void Player::update(std::chrono::milliseconds deltaTime) {
    using namespace std::chrono_literals;

    controller.update(deltaTime);

    direction = controller.direction();
    if (glm::length(direction) != 0)
        walkDuration += deltaTime;
    else
        walkDuration = 0ms;

    const float seconds = .3f;
    auto factor = glm::min(1.f, walkDuration.count() / 1000.f / seconds);

    position += direction * (factor * speed * deltaTime.count() / 1000.f);
}

void Player::setInputController(InputController controller) {
    Player::controller = controller;
}
