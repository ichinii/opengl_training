#ifndef OPENGL_TRAINING_GLM_H
#define OPENGL_TRAINING_GLM_H

#include <ostream>
#include <glm/glm.hpp>

std::ostream& operator<<(std::ostream& os, const glm::vec2& v)
{
    return os << "(" << v.x << ", " << v.y << ")";
}

#endif //OPENGL_TRAINING_GLM_H
