#version 330 core
layout (location = 0) in vec3 pos;
layout (location = 1) in vec3 color;
layout (location = 2) in vec3 normal;

uniform mat4 model;
uniform mat4 projection;
uniform mat4 view;

out vec3 vert_color;
out vec3 vert_normal;
out vec3 vert_world_pos;

void main()
{
    vec4 world_pos = model * vec4(pos, 1.0);
    vert_color = color;
    vert_normal = normal;
    vert_world_pos = world_pos.xyz;

    gl_Position = projection * view * world_pos;
}
