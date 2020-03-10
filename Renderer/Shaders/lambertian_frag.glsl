#version 450

#define MAX_POINT_LIGHTS 20
#define MAX_SPOT_LIGHTS 20

struct light
{
	vec3 ambient_colour;
	vec3 diffuse_colour;
	vec3 specular_colour;
	
	float ambient_intensity;
	float diffuse_intensity;
	float specular_intensity;
};

struct directional_light
{
	light base;
	
	vec3 direction;
};

struct point_light
{
	light base;
	
	vec3 position;
	
	float constant;
	float linear;
	float exponent;
};

struct material
{
	sampler2D bump_map;
	sampler2D albedo_map;

	vec3 ambient_colour;
	vec3 diffuse_colour;
	vec3 specular_colour;
	
	float specular_shininess;
};

uniform directional_light uniform_directional_light;
uniform point_light point_lights[MAX_POINT_LIGHTS];
uniform int point_light_count;

uniform material uniform_material;

uniform vec3 camera_position;

in vec2 final_texture_coords;
in vec3 final_average_normals;
in vec4 final_position;
in mat3 final_tangent_space;

out vec4 final_fragment_colour;

vec3 calculate_directional_lights();
vec3 calculate_point_lights();
vec3 calculate_directional_light(light a_light, vec3 a_direction);
vec3 calculate_point_light(point_light a_point_light);

void main()
{	
	// Sample passed textures.
	vec3 final_albedo_map = texture(uniform_material.albedo_map, final_texture_coords).rgb;
	
	// Calculate all lights in the scene.
	vec3 final_light_colour = calculate_directional_lights();
	final_light_colour += calculate_point_lights();
	
	final_fragment_colour = vec4(final_albedo_map * final_light_colour, 1.0f);// * 0.5 + vec3(0.5)
}

vec3 calculate_directional_lights()
{
	return calculate_directional_light(uniform_directional_light.base, uniform_directional_light.direction);
}

vec3 calculate_point_lights()
{
	vec3 total_colour = vec3(0.0, 0.0, 0.0);
	
	for (int i = 0; i < point_light_count; i++)
	{
		total_colour += calculate_point_light(point_lights[i]);
	}
	
	return total_colour;
}

vec3 calculate_directional_light(light a_light, vec3 a_direction)
{
	// Retrieve the bump map to use in calculating lighting.
	vec3 final_bump_map = texture(uniform_material.bump_map, final_texture_coords).rgb;
		
	// Calculate the ambient.
	//vec3 ambient_colour = (a_light.ambient_colour + uniform_material.ambient_colour) * a_light.ambient_intensity;
	
	// Normalize direction and average normals.
	vec3 normals = final_tangent_space * (final_bump_map * 2.0 - 1.0);
	a_direction = normalize(a_direction);
	
	// Calculate lambert term (negate the lights direction).
	float lambert_term = max(0.0f, min(1.0f, dot(normals, -a_direction)));
	
	// Calculate the diffuse.
	vec3 diffuse_colour = (a_light.diffuse_colour * uniform_material.diffuse_colour // Diffuse colours multiplied,
						   * lambert_term  		)	;								// Lambert term multiplied,
						   //* a_light.diffuse_intensity); 							// Intensity of the diffuse colour.
	
	// Only calculate specular as long as there is diffuse lighting.
	vec3 specular_colour = vec3(0.0f, 0.0f, 0.0f);
	if (lambert_term > 0.0f)
	{
		// Calculate view vector and reflection vector.
		vec3 frag_to_cam = normalize(camera_position - final_position.xyz);
		vec3 reflection = reflect(a_direction, normals);
		
		// Calculate specular term.
		float specular_term = pow(max(0.0f, dot(reflection, frag_to_cam)), uniform_material.specular_shininess);
		
		// Calculate the specular.
		specular_colour = vec3(a_light.specular_colour * uniform_material.specular_colour // Specular colours multiplied,
							   * specular_term 											  // Specular term multiplied,
							   * a_light.specular_intensity); 							  // Intensity of the specular colour.
	}
						  
	
	//return (ambient_colour + diffuse_colour + specular_colour);
	return (diffuse_colour + specular_colour);
}

vec3 calculate_point_light(point_light a_point_light)
{
	vec3 direction = a_point_light.position - final_position.xyz;
	float distance = length(direction);
	direction = normalize(-direction);
	
	vec3 colour = calculate_directional_light(a_point_light.base, direction);
	
	float attenuation = a_point_light.constant +					  // c +
						a_point_light.linear * distance +			  // bx +
						a_point_light.exponent * distance * distance; // ax^2
						
	return (colour / attenuation);
}