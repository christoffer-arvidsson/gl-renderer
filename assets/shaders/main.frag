#version 330 core
in vec3 vert_color;
in vec3 vert_normal;
in vec3 vert_world_pos;

uniform vec3 view_pos;

out vec4 frag_color;

void main() {
    vec3 light_color = vec3(1.0, 1.0, 1.0);
    vec3 light_world_pos = vec3(50.0, 50.0, 50.0);
    float ambient_strength = 0.1;
    float specular_strength = 0.5;
    float shininess = 32.0;

    vec3 light_dir = normalize(light_world_pos - vert_world_pos);
    vec3 norm = normalize(vert_normal);
    float diffuse_component = max(dot(norm, light_dir), 0.0);

    vec3 view_dir = normalize(view_pos - vert_world_pos);
    vec3 reflect_dir = reflect(-light_dir, norm);
    float spec_component = specular_strength * pow(max(dot(view_dir, reflect_dir), 0.0), shininess);

    vec3 lighting = (ambient_strength + diffuse_component + spec_component) * light_color;
    vec3 result = lighting * vert_color;

    frag_color = vec4(result, 1.0);
}
