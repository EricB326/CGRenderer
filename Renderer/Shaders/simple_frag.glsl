#version 460

uniform vec4 colour = vec4(1.0, 1.0, 1.0, 1.0);

out vec4 final_colour;

void main()
{
	final_colour = colour;
}