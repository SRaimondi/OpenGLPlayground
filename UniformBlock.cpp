//
// Created by Simon on 19.07.18.
//

#include "UniformBlock.hpp"

#include <iostream>
#include <vector>

UniformBlockElementDescription::UniformBlockElementDescription(GLuint i, GLenum t, GLsizei s, GLint off)
        : index(i), type(t), size(s), offset(off) {}

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
        for (unsigned int i = 0; i < num_active_uniforms; ++i) {
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

        // Loop over all indices, retrieve description and store them in the map
        for (unsigned int i = 0; i < num_active_uniforms; ++i) {
            glGetActiveUniform(program_id, indices_u[i], max_size, &length, &size, &type, name);
            GL_CHECK();

            m_uniforms_map.emplace(
                    std::make_pair<std::string, UniformBlockElementDescription>(
                            name,
                            UniformBlockElementDescription(indices_u[i], type, size, offsets[i])));
        }

    } else {
        std::cout << "No active uniforms found in block\n";
    }
}

UniformBlock::UniformBlock(const GLuint program_id, const std::string& block_name)
        : m_block_index(0), m_binding_point(0), m_buffer_id(0) {
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
