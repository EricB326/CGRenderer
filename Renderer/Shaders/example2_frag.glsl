#version 450

uniform sampler2D crate_texture;

in vec2 final_texture_coords;
out vec4 final_colour;

void main()
{
	final_colour = texture(crate_texture, final_texture_coords) * vec4(1.0f, 0.7f, 0.0f, 1.0f);
}