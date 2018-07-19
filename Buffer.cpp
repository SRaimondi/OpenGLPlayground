//
// Created by Simon on 19.07.18.
//

#include "Buffer.hpp"

Buffer::Buffer(GLenum target, GLenum usage)
        : m_id(0), m_target(target), m_usage(usage), m_is_binded(false) {
    // Generate buffer
    glGenBuffers(1, &m_id);
    GL_CHECK();
}

void Buffer::destroy() {
    // Unbind buffer
    unbind();
    // Destroy
    glDeleteBuffers(1, &m_id);
}

void Buffer::allocateSpace(GLsizeiptr size) {
    // Bind buffer
    bind();
    // Allocate space
    glBufferData(m_target, size, nullptr, m_usage);
    GL_CHECK();
    // Unbind
    unbind();
}
