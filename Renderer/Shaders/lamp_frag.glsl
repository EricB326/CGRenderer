#version 450

uniform vec4 lamp_colour;

out vec4 final_colour;

void main()
{
	final_colour = lamp_colour;
}