#version 410

// Input color from vertex shader
in vec3 interpolated_colour;

// Fragment output colour
out vec4 frag_colour;

void main() {
	frag_colour = vec4(interpolated_colour, 1.0);
}
