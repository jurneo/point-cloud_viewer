#version 120

uniform mat4 mvp_matrix;
attribute vec3 vertex_position;
attribute vec3 vertex_color;

varying vec3 frag_color;

void main()
{
    gl_PointSize = 1.5;
    gl_Position = mvp_matrix*vec4(vertex_position, 1.0);
    frag_color = vertex_color;
}

