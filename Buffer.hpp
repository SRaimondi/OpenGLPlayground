//
// Created by Simon on 19.07.18.
//

#ifndef OPENGLPLAYGROUND_BUFFER_HPP
#define OPENGLPLAYGROUND_BUFFER_HPP

#include "GLUtils.hpp"
#include <vector>
#include <iostream>

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

    // Get buffer id
    inline GLuint getID() const noexcept {
        return m_id;
    }

    // Submit data to whole buffer
    template<typename T>
    void submitData(const std::vector<T>& data);

    // Initialise empty space
    void allocateSpace(GLsizeiptr size);

    template<typename T>
    void submitSubData(const std::vector<T>& data, GLintptr offset);

    // Check if buffer is binded
    inline bool isBinded() const noexcept {
        return m_is_binded;
    }

    // Bind / unbind buffer
    inline void bind() const {
        if (!isBinded()) {
            glBindBuffer(m_target, m_id);
            m_is_binded = true;
        }
    }

    inline void unbind() const {
        if (isBinded()) {
            glBindBuffer(m_target, 0);
            m_is_binded = false;
        }
    }
};

template<typename T>
void Buffer::submitData(const std::vector<T>& data) {
    if (isBinded()) {
        // Submit data
        glBufferData(m_target, data.size() * sizeof(T), data.data(), m_usage);
        GL_CHECK();
    } else {
        std::cerr << "Trying to submit data to unbinded buffer\n";
    }
}

template<typename T>
void Buffer::submitSubData(const std::vector<T>& data, GLintptr offset) {
    if (isBinded()) {
        // Submit data
        glBufferSubData(m_target, offset, data.size() * sizeof(T), data.data());
        GL_CHECK();
    } else {
        std::cerr << "Trying to submit data to unbinded buffer\n";
    }
}

#endif //OPENGLPLAYGROUND_BUFFER_HPP
