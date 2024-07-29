#version 330 core

uniform vec2 aa_radius;

in vec4 v_col;
in float v_u;
in float v_v;
in float v_line_width;
in float v_line_length;

out vec4 frag_color;
void main()
{
    float au = 1.0 - smoothstep( 1.0 - ((2.0*aa_radius[0]) / v_line_width),  1.0, abs( v_u / v_line_width ) );
    float av = 1.0 - smoothstep( 1.0 - ((2.0*aa_radius[1]) / v_line_length), 1.0, abs( v_v / v_line_length ) );
    frag_color = v_col;
    frag_color.a *= min(au, av);
}
