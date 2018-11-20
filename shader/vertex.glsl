#version 450 core

uniform mat4 mvp;
layout (location = 0) in vec2 position;
layout (location = 1) in vec2 uv;
out vec2 varriant_uv;

void main()
{
    gl_Position = mvp * vec4(position, 0, 1);
    varriant_uv = vec2(uv.x, 1 - uv.y);
}
