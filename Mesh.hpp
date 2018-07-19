//
// Created by Simon on 18.07.18.
//

#ifndef OPENGLPLAYGROUND_MESH_HPP
#define OPENGLPLAYGROUND_MESH_HPP

#include "Shader.hpp"
#include "Buffer.hpp"

// Vertex data
struct Vertex {
    glm::vec3 position;
    glm::vec3 normal;
};

// Mesh class abstraction
class Mesh {
private:
    // VAO index
    GLuint m_vao;
    // Vertices and normal buffer
    Buffer m_vertices;
    // Indices buffer
    Buffer m_indices;
    // Number of indices
    std::size_t m_num_elements;

    // Setup mesh, initialises buffers and copies data
    void setupMesh(const std::vector<Vertex>& vertices, const std::vector<GLuint>& indices);

public:
    // Construct mesh from given host data
    Mesh(const std::vector<Vertex>& vertices, const std::vector<GLuint>& indices);

    // Destroy mesh
    void destroy();

    // Draw mesh
    void draw() const;
};

#endif //OPENGLPLAYGROUND_MESH_HPP
