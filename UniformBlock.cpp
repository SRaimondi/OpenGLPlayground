//
// Created by Simon on 19.07.18.
//

#include "UniformBlock.hpp"

#include <iostream>
#include <vector>

UniformBlockElementDescription::UniformBlockElementDescription(GLuint i, GLenum t, GLsizei s_b, GLsizei s, GLint off)
        : index(i), type(t), size_bytes(s_b), size(s), offset(off) {}

void UniformBlockElementDescription::printInformations() const {
    std::cout << "\tIndex: " << index << "\n";
    std::cout << "\tType: " << GLTypeToString(type) << "\n";
    std::cout << "\tSize (bytes): " << size_bytes << "\n";
    std::cout << "\tSize: " << size << "\n";
    std::cout << "\tOffset: " << offset << "\n";
}

void UniformBlock::setupMap(const GLuint program_id) {
    // Query how many uniforms are in the block
    GLint num_active_uniforms;
    glGetActiveUniformBlockiv(program_id, m_block_index, GL_UNIFORM_BLOCK_ACTIVE_UNIFORMS, &num_active_uniforms);
    GL_CHECK();
    if (num_active_uniforms > 0) {
        // Query the indices of the uniforms
        std::vector<GLint> indices(num_active_uniforms);
        glGetActiveUniformBlockiv(program_id, m_block_index, GL_UNIFORM_BLOCK_ACTIVE_UNIFORM_INDICES, indices.data());
        GL_CHECK();

        // Convert to GLuint
        std::vector<GLuint> indices_u(num_active_uniforms);
        for (int i = 0; i < num_active_uniforms; ++i) {
            indices_u[i] = static_cast<GLuint>(indices[i]);
        }

        // Get offsets of the uniforms
        std::vector<GLint> offsets(num_active_uniforms);
        glGetActiveUniformsiv(program_id, num_active_uniforms, indices_u.data(), GL_UNIFORM_OFFSET, offsets.data());
        GL_CHECK();

        // Get names of uniforms
        constexpr GLsizei max_size = 64;
        GLsizei length;
        GLint size;
        GLenum type;
        GLchar name[max_size];
        GLchar *final_name;

        // Loop over all indices, retrieve description and store them in the map
        for (int i = 0; i < num_active_uniforms; ++i) {
            glGetActiveUniform(program_id, indices_u[i], max_size, &length, &size, &type, name);
            GL_CHECK();

            // Check if uniform is an array and modify name
            if (size > 1) {
                final_name = strtok(name, "[");
            } else {
                final_name = name;
            }
            m_uniforms_map.emplace(
                    std::make_pair<std::string, UniformBlockElementDescription>(
                            final_name,
                            UniformBlockElementDescription(indices_u[i], type, GLTypeToSize(type), size,
                                                           offsets[i])));
        }

    } else {
        std::cout << "No active uniforms found in block\n";
    }
}

UniformBlock::UniformBlock()
        : m_block_index(0), m_binding_point(0), m_block_size(0) {}

UniformBlock::UniformBlock(const GLuint program_id, const std::string& block_name)
        : m_block_index(0), m_binding_point(0), m_block_size(0) {
    // First, find the index of the uniform block in the program
    m_block_index = glGetUniformBlockIndex(program_id, block_name.c_str());
    if (m_block_index == GL_INVALID_INDEX) {
        std::cerr << "Error getting uniform block index\n";
        exit(EXIT_FAILURE);
    }
    // Get the assigned binding point
    glGetActiveUniformBlockiv(program_id, m_block_index, GL_UNIFORM_BLOCK_BINDING, &m_binding_point);
    GL_CHECK();

    // Get size of the block
    glGetActiveUniformBlockiv(program_id, m_block_index, GL_UNIFORM_BLOCK_DATA_SIZE, &m_block_size);
    GL_CHECK();

    // Setup map
    setupMap(program_id);
}

void UniformBlock::printInformations() const {
    for (const auto& uniform : m_uniforms_map) {
        std::cout << "Name: " << uniform.first << "\n";
        uniform.second.printInformations();
    }
}

