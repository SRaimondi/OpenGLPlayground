//
// Created by Simon on 18.07.18.
//

#include "Mesh.hpp"

void Mesh::setupMesh(const std::vector<Vertex>& vertices, const std::vector<GLuint>& indices) {
    // Generate buffers
    glGenVertexArrays(1, &m_vao);

    // Bind VAO
    glBindVertexArray(m_vao);

    // Bind buffer
    m_vertices.bind();
    m_indices.bind();

    // Copy data to GPU
    m_vertices.submitData(vertices);
    m_indices.submitData(indices);

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
    // CAREFUL The vertex array object MUST be unbinded before the buffers or it will lose the automatic binding
    m_vertices.unbind();
    m_indices.unbind();

    GL_CHECK();
}

Mesh::Mesh(const std::vector<Vertex>& vertices, const std::vector<GLuint>& indices)
        : m_vao(0), m_vertices(GL_ARRAY_BUFFER, GL_STATIC_DRAW), m_indices(GL_ELEMENT_ARRAY_BUFFER, GL_STATIC_DRAW),
          m_num_elements(static_cast<GLsizei>(indices.size())) {
    setupMesh(vertices, indices);
}

void Mesh::destroy() {
    glDeleteVertexArrays(1, &m_vao);
    m_vertices.destroy();
    m_indices.destroy();
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
