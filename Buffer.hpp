//
// Created by Simon on 19.07.18.
//

#ifndef OPENGLPLAYGROUND_BUFFER_HPP
#define OPENGLPLAYGROUND_BUFFER_HPP

#include "GLUtils.hpp"
#include <vector>

class Buffer {
private:
    // Buffer identifier
    GLuint m_id;
    // Buffer type
    GLenum m_target;
    // Buffer usage
    GLenum m_usage;
    // Bind bool
    mutable bool m_is_binded;

public:
    Buffer(GLenum target, GLenum usage);

    // Destroy buffer
    void destroy();

    // Submit data to whole buffer
    template<typename T>
    void submitData(const std::vector<T>& data);

    // Initialise empty space
    void allocateSpace(GLsizeiptr size);

    template<typename T>
    void submitSubData(const std::vector<T>& data, GLintptr offset);

    // Bind / unbind buffer
    inline void bind() const {
        if (!m_is_binded) {
            glBindBuffer(m_target, m_id);
            m_is_binded = true;
        }
    }

    inline void unbind() const {
        if (m_is_binded) {
            glBindBuffer(m_target, 0);
            m_is_binded = false;
        }
    }
};

template<typename T>
void Buffer::submitData(const std::vector<T>& data) {
    // Bind buffer
    bind();
    // Submit data
    glBufferData(m_target, data.size() * sizeof(T), data.data(), m_usage);
    GL_CHECK();
    // Unbind
    unbind();
}

template<typename T>
void Buffer::submitSubData(const std::vector<T>& data, GLintptr offset) {
    // Bind buffer
    bind();
    // Submit data
    glBufferSubData(m_target, offset, data.size() * sizeof(T), data.data());
    GL_CHECK();
    // Unbind
    unbind();
}

#endif //OPENGLPLAYGROUND_BUFFER_HPP
