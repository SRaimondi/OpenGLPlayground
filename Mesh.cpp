//
// Created by Simon on 18.07.18.
//

#include "Mesh.hpp"

void Mesh::setupMesh(const std::vector<Vertex>& vertices, const std::vector<GLuint>& indices) {
    // Generate buffers
    glGenVertexArrays(1, &m_vao);
    glGenBuffers(1, &m_vbo);
    glGenBuffers(1, &m_ebo);

    // Bind VAO
    glBindVertexArray(m_vao);

    // Copy data to GPU
    glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), vertices.data(), GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(GLuint), indices.data(), GL_STATIC_DRAW);

    // Set attribute for position
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0,
                          3, GL_FLOAT, GL_FALSE,
                          sizeof(Vertex), reinterpret_cast<const void *>(offsetof(Vertex, position)));

    // Set attribute for normal
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1,
                          3, GL_FLOAT, GL_FALSE,
                          sizeof(Vertex), reinterpret_cast<const void *>(offsetof(Vertex, normal)));

    // Unbind
    glBindVertexArray(0);
    GL_CHECK();
}

Mesh::Mesh(const std::vector<Vertex>& vertices, const std::vector<GLuint>& indices)
        : m_vao(0), m_vbo(0), m_ebo(0), m_num_elements(static_cast<GLsizei>(indices.size())) {
    setupMesh(vertices, indices);
}

void Mesh::destroy() {
    glDeleteVertexArrays(1, &m_vao);
    glDeleteBuffers(1, &m_vbo);
    glDeleteBuffers(1, &m_ebo);
}

void Mesh::draw() const {
    // Bind VAO
    glBindVertexArray(m_vao);
    // Draw command
    glDrawElements(GL_TRIANGLES, m_num_elements, GL_UNSIGNED_INT, nullptr);
    // Unbind VAO
    glBindVertexArray(0);
    GL_CHECK();
}
