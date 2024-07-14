#version 330 core
in vec3 pos;
out vec4 color;

void main() {
    color = vec4((pos.x + 1.0)  / 2, (pos.y + 1.0)  / 2.0, (pos.z + 1.0)  / 2.0, 1.0);
}
