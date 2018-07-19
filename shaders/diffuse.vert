#version 410

// Array attributes
layout (location = 0) in vec3 vertex_position;
layout (location = 1) in vec3 vertex_normal;

// Matrices
//uniform mat4 model;
//uniform mat4 view;
//uniform mat4 proj;

uniform Matrices {
    mat4 model;
    mat4 view;
    mat4 proj;
};

out VS_OUT {
    // Vertex and normal in camera space
    vec3 vertex_camera;
    vec3 normal_camera;
} vs_out;

void main() {
    // Compute output position
	gl_Position = proj * view * model * vec4(vertex_position, 1.0);
	mat3 normal = transpose(inverse(mat3(view * model)));
	// Compute variables in camera space
	vs_out.vertex_camera = (view * model * vec4(vertex_position, 1.0)).xyz;
	vs_out.normal_camera = normalize(normal * vertex_normal);
}