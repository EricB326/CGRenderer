#version 450

layout (location = 0) in vec3 local_position;
layout (location = 1) in vec3 colour;


uniform mat4 projection_view_matrix;
uniform mat4 model_matrix;

out vec3 out_colour;

void main()
{
	out_colour = colour;
	
	vec3 new_position = local_position;
					
	gl_Position = ((projection_view_matrix * model_matrix) * vec4(new_position, 1.0f));
}