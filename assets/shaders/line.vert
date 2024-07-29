#version 450

struct Vertex {
    vec4 position;
    vec4 color;
    vec4 normal;
};

layout(shared, binding = 0) buffer vertex_buffer { Vertex vertices[]; };

uniform mat4 view;
uniform mat4 projection;

uniform vec2 viewport_size;
uniform vec2 aa_radius;
uniform float thickness;

out vec4 v_col;
out float v_u;
out float v_v;
out float v_line_width;
out float v_line_length;

void main() {
    mat4 pvm = projection * view;

    float width = viewport_size[0];
    float height = viewport_size[1];

    float aspect_ratio = height / width;

    int line_id_0 = (gl_VertexID / 6) * 2;
    int line_id_1 = line_id_0 + 1;
    int quad_id = gl_VertexID % 6;
    ivec2 quad[6] = {ivec2(0, -1), ivec2(0, 1), ivec2(1, 1),
        ivec2(0, -1), ivec2(1, 1), ivec2(1, -1)};

    Vertex line_vertices[2] = {vertices[line_id_0], vertices[line_id_1]};

    vec4 clip_pos_a = pvm * vec4(line_vertices[0].position.xyz, 1.0);
    vec4 clip_pos_b = pvm * vec4(line_vertices[1].position.xyz, 1.0);

    vec2 ndc_pos_a = clip_pos_a.xy / clip_pos_a.w;
    vec2 ndc_pos_b = clip_pos_b.xy / clip_pos_b.w;

    vec2 line_vector = ndc_pos_b - ndc_pos_a;
    vec2 viewport_line_vector = line_vector * viewport_size;
    vec2 dir = normalize(vec2(line_vector.x, line_vector.y * aspect_ratio));

    float extension_length = (aa_radius.y);
    float line_length = length(viewport_line_vector) + 2.0 * extension_length;
    float line_width_a = max(thickness, 1.0) + aa_radius.x;
    float line_width_b = max(thickness, 1.0) + aa_radius.x;

    vec2 normal = vec2(-dir.y, dir.x);
    vec2 normal_a = vec2(line_width_a / width, line_width_a / height) * normal;
    vec2 normal_b = vec2(line_width_b / width, line_width_b / height) * normal;
    vec2 extension =
        vec2(extension_length / width, extension_length / height) * dir;

    ivec2 quad_pos = quad[quad_id];

    v_line_width = (1.0 - quad_pos.x) * line_width_a + quad_pos.x * line_width_b;
    v_line_length = 0.5 * line_length;
    v_v = (2.0 * quad_pos.x - 1.0) * v_line_length;
    v_u = (quad_pos.y) * v_line_width;

    vec2 zw_part =
        (1.0 - quad_pos.x) * clip_pos_a.zw + quad_pos.x * clip_pos_b.zw;
    vec2 dir_y =
        quad_pos.y * ((1.0 - quad_pos.x) * normal_a + quad_pos.x * normal_b);
    vec2 dir_x = quad_pos.x * line_vector + (2.0 * quad_pos.x - 1.0) * extension;

    v_col = line_vertices[quad_pos.x].color;
    v_col.a = min(thickness * v_col.a, 1.0f);

    gl_Position = vec4((ndc_pos_a + dir_x + dir_y) * zw_part.y, zw_part);
}
