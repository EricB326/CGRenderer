#version 450

in vec3 out_colour;

out vec4 final_colour;

void main()
{
	final_colour = vec4(out_colour, 1.0f);
}