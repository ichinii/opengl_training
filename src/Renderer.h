#ifndef OPENGL_TRAINING_RENDERER_H
#define OPENGL_TRAINING_RENDERER_H

#include <glm/glm.hpp>
#include <vector>
#include <GL/glew.h>

class Renderer {
    std::vector<glm::vec2> positions;
    std::vector<glm::vec2> uvs;
    glm::mat4 mvp;
    GLuint vao;
    GLuint vbos[2];

public:
    Renderer();
    ~Renderer();
    void drawTile(glm::vec2 position, glm::vec2 size, unsigned char tile);
    void setMvp(glm::mat4 mvp);
    void clear();
    void flush();
};


#endif //OPENGL_TRAINING_RENDERER_H
