//
// Created by Simon on 19.07.18.
//

#ifndef OPENGLPLAYGROUND_UNIFORMBLOCK_HPP
#define OPENGLPLAYGROUND_UNIFORMBLOCK_HPP

#include <map>
#include "GLUtils.hpp"

// Uniform block element description
struct UniformBlockElementDescription {
    // Index
    GLuint index;
    // Type of the variable
    GLenum type;
    // Size of the variable
    GLsizei size;
    // Offset
    GLint offset;

    UniformBlockElementDescription(GLuint i, GLenum t, GLsizei s, GLint off);
};

// Uniform block wrapper class associated with a program
class UniformBlock {
private:
    // Block index in shader
    GLuint m_block_index;
    // Binding point
    GLint m_binding_point;
    // Buffer index
    GLuint m_buffer_id;

    // Size of the buffer
    GLint m_block_size;

    // Map between uniform's names and description
    std::map<std::string, UniformBlockElementDescription> m_uniforms_map;

    // Setup the uniforms map
    void setupMap(GLuint program_id);

public:
    UniformBlock(GLuint program_id, const std::string& block_name);
};

#endif //OPENGLPLAYGROUND_UNIFORMBLOCK_HPP
