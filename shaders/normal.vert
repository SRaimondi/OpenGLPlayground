#version 410

// Array attributes
layout (location = 0) in vec3 vertex_position;
layout (location = 1) in vec3 vertex_normal;

// Matrices uniform block
uniform Matrices {
    mat4 view;
    mat4 proj;
};

// Model matrix
uniform mat4 model;

// Output normal
out vec3 interp_normal;

void main() {
    // Compute output position
	gl_Position = proj * view * model * vec4(vertex_position, 1.0);
	// Compute output normal
	interp_normal = normalize(transpose(inverse(mat3(model))) * vertex_normal);
}
