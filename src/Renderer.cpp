#include "Renderer.h"

Renderer::Renderer() {
    // generate buffers
    glGenBuffers(2, vbos);
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);

    // init position buffer
    glEnableVertexAttribArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, vbos[0]);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, nullptr);

    // init uv buffer
    glEnableVertexAttribArray(1);
    glBindBuffer(GL_ARRAY_BUFFER, vbos[1]);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, nullptr);

    glBindVertexArray(0);
}

Renderer::~Renderer() {

}

void Renderer::drawTile(glm::vec2 position, glm::vec2 size, unsigned char tile) {

}

void Renderer::setMvp(glm::mat4 mvp) {

}

void Renderer::clear() {

}

void Renderer::flush() {

}
