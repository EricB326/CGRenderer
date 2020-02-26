#version 450

uniform sampler2D object_texture;

in vec2 final_texture_coords;
out vec4 final_colour;

void main()
{
	final_colour = texture(object_texture, final_texture_coords);
}