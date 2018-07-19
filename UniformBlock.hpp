//
// Created by Simon on 19.07.18.
//

#ifndef OPENGLPLAYGROUND_UNIFORMBLOCK_HPP
#define OPENGLPLAYGROUND_UNIFORMBLOCK_HPP

#include "GLUtils.hpp"

#include <unordered_map>
#include <iostream>

// Uniform block element description
struct UniformBlockElementDescription {
    // Index
    GLuint index;
    // Type of the variable
    GLenum type;
    // Size of the variable in bytes
    GLsizei size_bytes;
    // Size of the variable (for arrays this is > 1)
    GLsizei size;
    // Offset
    GLint offset;

    UniformBlockElementDescription(GLuint i, GLenum t, GLsizei s_b, GLsizei s, GLint off);

    // Print structure
    void printInformations() const;
};

// Uniform block wrapper class associated with a program
class UniformBlock {
private:
    // Block index in shader
    GLuint m_block_index;
    // Binding point
    GLint m_binding_point;

    // Size of the buffer
    GLint m_block_size;

    // Map between uniform's names and description
    std::unordered_map<std::string, UniformBlockElementDescription> m_uniforms_map;

    // Setup the uniforms map
    void setupMap(GLuint program_id);

public:
    UniformBlock();

    // Construct UniformBlock for a given program and given the name
    UniformBlock(GLuint program_id, const std::string& block_name);

    // Get description given name
    inline const UniformBlockElementDescription& operator()(const std::string& uniform_name) const {
        const auto it = m_uniforms_map.find(uniform_name);
        if (it == m_uniforms_map.end()) {
            std::cerr << "Requesting invalid uniform description\n";
            exit(EXIT_FAILURE);
        } else {
            return it->second;
        }
    }

    // Access uniform block data
    inline GLuint getIndex() const noexcept {
        return m_block_index;
    }

    inline GLuint getBindingPoint() const noexcept {
        return m_binding_point;
    }

    inline GLint getBlockSize() const noexcept {
        return m_block_size;
    }

    // Print information about the block
    void printInformations() const;
};

#endif //OPENGLPLAYGROUND_UNIFORMBLOCK_HPP
