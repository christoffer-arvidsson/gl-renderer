#version 330 core
in vec3 vert_color;
in vec3 vert_normal;
in vec3 vert_world_pos;

uniform vec3 view_pos;

struct Material {
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
    float shininess;
};

struct Light {
    vec3 position;
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

uniform Material material;
uniform Light light;

out vec4 frag_color;

void main() {
    vec3 ambient_component = light.ambient * material.ambient;

    vec3 light_dir = normalize(light.position - vert_world_pos);
    vec3 norm = normalize(vert_normal);
    vec3 diffuse_component = light.diffuse * material.diffuse * max(dot(norm, light_dir), 0.0);

    vec3 view_dir = normalize(view_pos - vert_world_pos);
    vec3 reflect_dir = reflect(-light_dir, norm);
    vec3 spec_component = light.specular * material.specular * pow(max(dot(view_dir, reflect_dir), 0.0), material.shininess);

    vec3 lighting = (ambient_component + diffuse_component + spec_component);
    vec3 result = lighting * vert_color;

    frag_color = vec4(result, 1.0);
}
