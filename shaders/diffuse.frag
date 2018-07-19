#version 410

in VS_OUT {
    // Vertex and normal in camera space
    vec3 vertex_camera;
    vec3 normal_camera;
} fs_in;

// Output fragment color
out vec4 frag_color;

void main() {
    // Compute color based on normal and camera position
    float n_dot_dir = dot(normalize(-fs_in.vertex_camera), fs_in.normal_camera);
    frag_color = vec4(n_dot_dir, n_dot_dir, n_dot_dir, 1.f);
}
