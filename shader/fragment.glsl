#version 450 core

in vec2 varriant_uv;

uniform sampler2D tex;

out vec4 fragColor;

void main() {
    fragColor = texture2D(tex, varriant_uv);
}
