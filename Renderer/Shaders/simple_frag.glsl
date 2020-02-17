#version 450

uniform vec4 colour = vec4(1.0, 1.0, 1.0, 1.0);

in vec3 outNormals;

out vec4 final_colour;

void main()
{
	final_colour = vec4(outNormals, 1.0);
}