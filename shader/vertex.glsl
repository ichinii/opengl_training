#version core 450

uniform mat4 mvp;
layout(location = 0) vec2 position;
layout(location = 1) vec2 uv;
out vec2 varriant_uv;

void main() {
    gl_Position = mvp * vec4(position, 0, 1);
    varriant_uv = uv;
}