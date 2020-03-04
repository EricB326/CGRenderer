#version 450

layout (location = 0) in vec4 local_position;
layout (location = 1) in vec4 normals;
layout (location = 2) in vec4 tangents;
layout (location = 3) in vec2 texture_coords;

uniform mat4 projection_view_matrix;
uniform mat4 projection_matrix;
uniform mat4 view_matrix;
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
	
	final_average_normals = normal_matrix * normals.xyz;
	
	final_position = model_matrix * local_position;
	
	final_tangent_space = calculate_tbn_matrix();

	vec4 new_position = local_position;
	//new_position.x += ((sin(local_position.y * 3.0f + time * 3.0f) * 0.7f));
					
	gl_Position = ((projection_view_matrix * model_matrix) * vec4(new_position));
}

mat3 calculate_tbn_matrix()
{
	vec3 tangent = normal_matrix * tangents.xyz;
	vec3 normal = normal_matrix * normals.xyz;
	vec3 bitangent = cross(tangent, normal) * tangents.w;
	
	tangent = normalize(tangent);
	normal = normalize(normal);
	bitangent = normalize(bitangent);
	
	//vec3 tangent = normalize(vec3(a_normal_matrix * tangents.xyz));
	//vec3 normal =  normalize(vec3(a_normal_matrix * normals.xyz));
	//vec3 bitangent = cross(tangent, normal) * tangents.w;
	//bitangent = normalize(vec3(a_normal_matrix * vec4(bitangent, 0.0f)));
	
	mat3 tangent_space = mat3(tangent, bitangent, normal);
	return tangent_space;
}