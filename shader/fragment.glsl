#version core 450

in vec2 varriant_uv;
uniform sampler2D tex;
out vec4 gl_FragColor;

void main() {
    gl_FragColor = texture2D(tex, varriant_uv);
}