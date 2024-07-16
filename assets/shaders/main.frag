#version 330 core
in vec3 pos_out;
out vec4 color;

void main() {
    color = vec4(1.0f, 1.0f, pos_out.z / 10.0f, 1.0f);
}
