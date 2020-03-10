#version 450

layout (location = 0) in vec4 local_position;
layout (location = 1) in vec4 normals;
layout (location = 2) in vec4 tangents;
layout (location = 3) in vec2 texture_coords;

uniform mat4 projection_view_matrix;
uniform mat4 model_matrix;
uniform mat3 normal_matrix;
uniform float time = 0.0f;

out vec2 final_texture_coords;
out vec3 final_average_normals;
out vec4 final_position;
out mat3 final_tangent_space;

mat3 calculate_tbn_matrix();

void main()
{	
	final_texture_coords = texture_coords;	
	
	final_average_normals = normalize(vec3(model_matrix * vec4(normals.xyz, 0)));  //normalize(normal_matrix * normals.xyz);
	
	final_position = model_matrix * local_position;
	
	final_tangent_space = calculate_tbn_matrix();

	vec4 new_position = local_position;
	//new_position.x += ((sin(local_position.y * 3.0f + time * 3.0f) * 0.7f));
					
	gl_Position = ((projection_view_matrix * model_matrix) * vec4(new_position));
}

mat3 calculate_tbn_matrix()
{
	vec3 tangent = normalize(tangents.xyz);
	vec3 normal = final_average_normals;
	vec3 bitangent = cross(normals.xyz, tangents.xyz) * tangents.w;;
	bitangent = normalize(-bitangent);
	
	mat3 tangent_space = mat3(tangent, bitangent, normal);
	return tangent_space;
}