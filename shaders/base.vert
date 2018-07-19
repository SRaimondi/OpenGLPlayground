//#version 410
//
//// Input attributes
//layout (location = 0) in vec3 vertex_position;
//layout (location = 1) in vec3 vertex_colour;
//
//// Matrices uniforms
//uniform mat4 model;
//uniform mat4 view;
//uniform mat4 projection;
//
//// Output interpolated colour for fragment shader
//out vec3 interpolated_colour;
//
//void main() {
//    // Compute vertex position
//    gl_Position = projection * view * model * vec4(vertex_position, 1.0);
//    // Set output color
//    interpolated_colour = vertex_colour;
//}
