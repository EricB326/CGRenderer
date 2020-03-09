#version 450

#define MAX_POINT_LIGHTS 20
#define MAX_SPOT_LIGHTS 20

struct light
{
	vec3 light_ambient_colour;
	vec3 light_diffuse_colour;
	vec3 light_specular_colour;
	
	float light_ambient_intensity;
	float light_diffuse_intensity;
	float light_specular_intensity;
};

struct directional_light
{
	light base;
	vec3 light_direction;
};

struct material
{
	sampler2D bump_map;
	sampler2D diffuse_map;
	sampler2D specular_map;

	vec3 mat_ambient_colour;
	vec3 mat_diffuse_colour;
	vec3 mat_specular_colour;
	
	float mat_specular_shininess;
};

uniform directional_light uniform_directional_light;
uniform material uniform_material;

uniform vec3 camera_position;

in vec2 final_texture_coords;
in vec3 final_average_normals;
in vec4 final_position;
in mat3 final_tangent_space;

out vec4 final_fragment_colour;

vec3 calculate_directional_lights();
vec3 calculate_directional_light(light a_light, vec3 a_direction);

void main()
{	
	// Sample passed textures.
	vec3 final_diffuse_map = texture(uniform_material.diffuse_map, final_texture_coords).rgb;
	
	vec3 final_light_colour = calculate_directional_lights();
	
	final_fragment_colour = vec4(final_diffuse_map * final_light_colour, 1.0f);
}

vec3 calculate_directional_lights()
{
	return calculate_directional_light(uniform_directional_light.base, uniform_directional_light.light_direction);
}

vec3 calculate_directional_light(light a_light, vec3 a_direction)
{		
	vec3 final_bump_map = texture(uniform_material.bump_map, final_texture_coords).rgb;
	vec3 final_specular_map = texture(uniform_material.specular_map, final_texture_coords).rgb;
		
	// Calculate the ambient.
	vec3 ambient_colour = (a_light.light_ambient_colour + uniform_material.mat_ambient_colour) * a_light.light_ambient_intensity;
	
	// Normalize direction and average normals.
	vec3 normals = final_tangent_space * (final_bump_map * 2.0 - 1.0);
	a_direction = normalize(a_direction);
	
	// Calculate lambert term (negate the lights direction).
	float lambert_term = max(0.0f, min(1.0f, dot(normals, -a_direction)));
	
	// Calculate the diffuse.					   
	vec3 diffuse_colour = ((a_light.light_diffuse_colour * (uniform_material.mat_diffuse_colour // Diffuse colours multiplied,
						   * lambert_term))  													// Lambert term multiplied,
						   * a_light.light_diffuse_intensity);								    // Intensity of the diffuse colour.		   
	
	// Only calculate specular as long as there is diffuse lighting.
	vec3 specular_colour = vec3(0.0f, 0.0f, 0.0f);
	if (lambert_term > 0.0f)
	{
		// Calculate view vector and reflection vector.
		vec3 frag_to_cam = normalize(camera_position - final_position.xyz);
		vec3 reflection = reflect(a_direction, normals);
		
		// Calculate specular term.
		float specular_term = pow(max(0.0f, dot(reflection, frag_to_cam)), uniform_material.mat_specular_shininess);
		
		// Calculate the specular.
		specular_colour = vec3((a_light.light_specular_colour * uniform_material.mat_specular_colour // Specular colours multiplied,
							   * final_specular_map * specular_term) 								 // Specular map & specular term multiplied,
							   * a_light.light_specular_intensity); 								 // Intensity of the specular colour.
	}
						  
	
	return (ambient_colour + diffuse_colour + specular_colour);
}