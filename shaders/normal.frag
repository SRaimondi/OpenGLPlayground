#version 410

// Input normal
in vec3 interp_normal;

// Output fragment color
out vec4 frag_color;

void main() {
    // Compute color based on normal
	frag_color = vec4(abs(interp_normal), 1.0);
}
