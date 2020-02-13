#version 450

layout (location = 0) in vec4 local_position;
layout (location = 1) in vec4 normals;
layout (location = 2) in vec4 tangents;
layout (location = 3) in vec4 bitangents;
layout (location = 4) in vec2 texture_coords;

uniform mat4 projection_view_matrix;
uniform mat4 model_matrix;

void main()
{
	gl_Position = (projection_view_matrix * model_matrix) * vec4(local_position);
}