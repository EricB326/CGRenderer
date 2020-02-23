#version 450

layout (location = 0) in vec4 local_position;
layout (location = 1) in vec4 normals;
layout (location = 2) in vec4 tangents;
layout (location = 3) in vec4 bitangents;
layout (location = 4) in vec2 texture_coords;

uniform mat4 projection_view_matrix;
uniform mat4 model_matrix;
uniform float time = 0.0f;

out vec2 final_texture_coords;

void main()
{	
	final_texture_coords = texture_coords;	
	
	vec4 new_position = local_position;
	
	new_position.x += ((sin(local_position.y * 3.0f + time * 3.0f) * 0.7f));
					
	gl_Position = ((projection_view_matrix * model_matrix) * vec4(new_position));
}