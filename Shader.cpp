//
// Created by Simon on 17.07.18.
//

// Project files
#include "Shader.hpp"
#include "FileIO.hpp"
// Glm pointer wrapper
#include <glm/gtc/type_ptr.hpp>
// STL includes
#include <fstream>
#include <iostream>
#include <sstream>

std::string shaderTypeToString(const ShaderType& type) {
    switch (type) {
        case ShaderType::Vertex:
            return "Vertex";
        case ShaderType::Fragment:
            return "Fragment";
        case ShaderType::TessControl:
            return "Tessellation control";
        case ShaderType::TessEvaluation:
            return "Tessellation evaluation";
        case ShaderType::Geometry:
            return "Geometry";
    }
}

void Shader::compile() const {
    // Compile shader
    glCompileShader(m_shader_id);
    // Check for errors
    GLint params = -1;
    glGetShaderiv(m_shader_id, GL_COMPILE_STATUS, &params);
    if (params != GL_TRUE) {
        std::cerr << "##### Error during shader compilation #####\n";
#ifndef NDEBUG
        // Print log
        printShaderLog();
#endif
        exit(EXIT_FAILURE);
    }
    std::cout << shaderTypeToString(m_type) << " shader compiled successfully!\n";
}

void Shader::printShaderLog() const {
    // Get log length
    GLsizei len;
    glGetShaderiv(m_shader_id, GL_INFO_LOG_LENGTH, &len);
    // Get log
    auto log = new GLchar[len + 1];
    glGetShaderInfoLog(m_shader_id, len, &len, log);
    // Print log
    std::cerr << "Compiler LOG\n" << log << "\n";
    // Free memory
    delete[] log;
}

Shader::Shader(const std::string& file_name, const ShaderType& type)
        : m_shader_id(0), m_type(type) {
    // Code source
    const std::string vertex_source = loadFile(file_name);

    // Get pointer to source
    const GLchar *source = vertex_source.c_str();

    // Create shader
    m_shader_id = glCreateShader(static_cast<GLenum>(m_type));
    GL_CHECK();

    // Set shader source
    glShaderSource(m_shader_id, 1, &source, nullptr);
    GL_CHECK();

    // Compile shader
    compile();
}

Shader::Shader(const std::vector<std::string>& sources, const ShaderType& type)
        : m_shader_id(0), m_type(type) {
    // Put source in OpenGL format
    std::vector<const GLchar *> char_sources;
    for (const auto& source : sources) {
        char_sources.emplace_back(source.c_str());
    }

    // Create shader
    m_shader_id = glCreateShader(static_cast<GLenum>(m_type));
    GL_CHECK();

    // Set shader source, last argument is nullptr since we assume the strings are null terminated
    glShaderSource(m_shader_id, static_cast<GLsizei>(sources.size()), char_sources.data(), nullptr);
    GL_CHECK();

    // Compile shader
    compile();
}

void Shader::destroy() {
    glDeleteShader(m_shader_id);
}

void Program::link() const {
    // Link program
    glLinkProgram(m_program_id);
    // Check for errors
    GLint params = -1;
    glGetProgramiv(m_program_id, GL_LINK_STATUS, &params);
    if (params != GL_TRUE) {
        std::cerr << "##### Error during program linking #####\n";
        // Print log
        printProgramInfoLog();
    }
    std::cout << "Program linked successfully!\n";
}

void Program::printProgramInfoLog() const {
    // Get log length
    GLsizei len;
    glGetProgramiv(m_program_id, GL_INFO_LOG_LENGTH, &len);
    // Get log
    auto log = new GLchar[len + 1];
    glGetProgramInfoLog(m_program_id, len, &len, log);
    // Print log
    std::cerr << "Linker LOG\n" << log << "\n";
    // Free memory
    delete[] log;
}

Program::Program(const std::initializer_list<Shader>& shaders)
        : m_program_id(0) {
    // Check that we have at least one vertex and fragment shader
    bool found_vertex = false;
    bool found_fragment = false;
    // Check that we are not passing multiple vertex or graf
    for (const auto& shader : shaders) {
        if (shader.getType() == ShaderType::Vertex) {
            found_vertex = true;
        } else if (shader.getType() == ShaderType::Fragment) {
            found_fragment = true;
        }
    }

    // If we don't find a vertex and a fragment shader, abort
    if (!found_vertex || !found_fragment) {
        std::cerr << "Did not find a vertex and fragment shader in program creation...\n";
        exit(EXIT_FAILURE);
    }

    // Create program
    m_program_id = glCreateProgram();
    GL_CHECK();

    // Attach shaders
    for (const auto& shader : shaders) {
        glAttachShader(m_program_id, shader.getID());
    }
    GL_CHECK();

    // Link program
    link();
}

void Program::destroy() {
    glDeleteProgram(m_program_id);
}

void Program::validate() const noexcept {
    // Validate program
    glValidateProgram(m_program_id);
    GL_CHECK();
    // Get validation report
    GLint params = -1;
    glGetProgramiv(m_program_id, GL_VALIDATE_STATUS, &params);
    std::cout << "Validation status: " << params << "\n";
    // If there is an error, print information log
    if (params != GL_TRUE) {
        printProgramInfoLog();
        exit(EXIT_FAILURE);
    }
}

GLint Program::getNumAttachedShaders() const noexcept {
    GLint num_shaders = -1;
    glGetShaderiv(m_program_id, GL_ATTACHED_SHADERS, &num_shaders);
    GL_CHECK();

    return num_shaders;
}

bool Program::prefetchUniform(const std::string& uniform_name) const {
    const GLint location = glGetUniformLocation(m_program_id, uniform_name.c_str());
    if (location == -1) {
        return false;
    } else {
        m_uniforms_map[uniform_name] = static_cast<GLuint>(location);
        return true;
    }
}

bool Program::prefetchUniforms(const std::initializer_list<std::string>& uniform_names) const {
    // Loop over all given uniforms and prefetch them
    for (const auto& u : uniform_names) {
        if (!prefetchUniform(u)) {
            return false;
        }
    }
    return true;
}

bool Program::prefetchUniformBlock(const std::string& uniform_block_name) const {
    // Create uniform block and insert it into map
    m_uniforms_block_map[uniform_block_name] = UniformBlock(m_program_id, uniform_block_name);
    return true;
}

bool Program::prefetchUniformBlocks(const std::initializer_list<std::string>& uniform_blocks_names) const {
    // Loop over all given uniform blocks and prefetch them
    for (const auto& ub : uniform_blocks_names) {
        if (!prefetchUniformBlock(ub)) {
            return false;
        }
    }
    return true;
}

bool Program::prefetchAttribute(const std::string& attribute_name) const {
    const GLint location = glGetAttribLocation(m_program_id, attribute_name.c_str());
    if (location == -1) {
        return false;
    } else {
        m_attributes_map[attribute_name] = static_cast<GLuint>(location);
        return true;
    }
}

bool Program::prefetchAttributes(const std::initializer_list<std::string>& attribute_names) const {
    // Loop over all given attributes and prefetch them
    for (const auto& a : attribute_names) {
        if (!prefetchAttribute(a)) {
            return false;
        }
    }
    return true;
}

GLuint Program::getUniformLocation(const std::string& uniform_name) const {
    // Check if the uniform has been fetched
    const auto it = m_uniforms_map.find(uniform_name);
    if (it == m_uniforms_map.end()) {
        // Try to fetch the value for later usage
        if (!prefetchUniform(uniform_name)) {
            std::cerr << "Program does not have a uniform called: " << uniform_name << "\n";
            return std::numeric_limits<GLuint>::max();
        } else {
            // Not the location is in the map, return it
            return m_uniforms_map[uniform_name];
        }
    } else {
        // Return location directly
        return it->second;
    }
}

const UniformBlock& Program::getUniformBlock(const std::string& uniform_block_name) const {
    // Check if the uniform block has been fetched
    const auto it = m_uniforms_block_map.find(uniform_block_name);
    if (it == m_uniforms_block_map.end()) {
        // Try to fetch the block for later usage
        if (prefetchUniformBlock(uniform_block_name)) {
            // Now the location is in the map, return it
            return m_uniforms_block_map[uniform_block_name];
        }
    } else {
        // Return uniform block directly
        return it->second;
    }
}

GLuint Program::getAttributeLocation(const std::string& attrib_name) const {
    // Check if the attribute has been fetched
    const auto it = m_attributes_map.find(attrib_name);
    if (it == m_attributes_map.end()) {
        // Try to fetch the value for later usage
        if (!prefetchAttribute(attrib_name)) {
            std::cerr << "Program does not have an attribute called: " << attrib_name << "\n";
            return std::numeric_limits<GLuint>::max();
        } else {
            // Now the location is in the map, return it
            return m_attributes_map[attrib_name];
        }
    } else {
        // Return location directly
        return it->second;
    }
}

void Program::setBool(const std::string& name, bool value) const {
    // Get location from map
    const GLint l = getUniformLocation(name);
    // In debug mode, check that location is valid
#ifndef NDEBUG
    if (l == -1) {
        std::cerr << "Invalid uniform location given\n";
        return;
    }
#endif
    glUniform1i(l, static_cast<GLint>(value));
}

void Program::setInt(const std::string& name, int value) const {
    // Get location from map
    const GLint l = getUniformLocation(name);
    // In debug mode, check that location is valid
#ifndef NDEBUG
    if (l == -1) {
        std::cerr << "Invalid uniform location given\n";
        return;
    }
#endif
    glUniform1i(l, value);
}

void Program::setFloat(const std::string& name, float value) const {
    // Get location from map
    const GLint l = getUniformLocation(name);
    // In debug mode, check that location is valid
#ifndef NDEBUG
    if (l == -1) {
        std::cerr << "Invalid uniform location given\n";
        return;
    }
#endif
    glUniform1f(l, value);
}

void Program::setVec2(const std::string& name, const glm::vec2& v) const {
    // Get location from map
    const GLint l = getUniformLocation(name);
    // In debug mode, check that location is valid
#ifndef NDEBUG
    if (l == -1) {
        std::cerr << "Invalid uniform location given\n";
        return;
    }
#endif
    glUniform2fv(l, 1, glm::value_ptr(v));
}

void Program::setVec2(const std::string& name, float x, float y) const {
    // Get location from map
    const GLint l = getUniformLocation(name);
    // In debug mode, check that location is valid
#ifndef NDEBUG
    if (l == -1) {
        std::cerr << "Invalid uniform location given\n";
        return;
    }
#endif
    glUniform2f(l, x, y);
}

void Program::setVec3(const std::string& name, const glm::vec3& v) const {
    // Get location from map
    const GLint l = getUniformLocation(name);
    // In debug mode, check that location is valid
#ifndef NDEBUG
    if (l == -1) {
        std::cerr << "Invalid uniform location given\n";
        return;
    }
#endif
    glUniform3fv(l, 1, glm::value_ptr(v));
}

void Program::setVec3(const std::string& name, float x, float y, float z) const {
    // Get location from map
    const GLint l = getUniformLocation(name);
    // In debug mode, check that location is valid
#ifndef NDEBUG
    if (l == -1) {
        std::cerr << "Invalid uniform location given\n";
        return;
    }
#endif
    glUniform3f(l, x, y, z);
}

void Program::setVec4(const std::string& name, const glm::vec4& v) const {
    // Get location from map
    const GLint l = getUniformLocation(name);
    // In debug mode, check that location is valid
#ifndef NDEBUG
    if (l == -1) {
        std::cerr << "Invalid uniform location given\n";
        return;
    }
#endif
    glUniform4fv(l, 1, glm::value_ptr(v));
}

void Program::setVec4(const std::string& name, float x, float y, float z, float w) const {
    // Get location from map
    const GLint l = getUniformLocation(name);
    // In debug mode, check that location is valid
#ifndef NDEBUG
    if (l == -1) {
        std::cerr << "Invalid uniform location given\n";
        return;
    }
#endif
    glUniform4f(l, x, y, z, w);
}

void Program::setMat2(const std::string& name, const glm::mat2& m) const {
    // Get location from map
    const GLint l = getUniformLocation(name);
    // In debug mode, check that location is valid
#ifndef NDEBUG
    if (l == -1) {
        std::cerr << "Invalid uniform location given\n";
        return;
    }
#endif
    glUniformMatrix2fv(l, 1, GL_FALSE, glm::value_ptr(m));
}

void Program::setMat3(const std::string& name, const glm::mat3& m) const {
    // Get location from map
    const GLint l = getUniformLocation(name);
    // In debug mode, check that location is valid
#ifndef NDEBUG
    if (l == -1) {
        std::cerr << "Invalid uniform location given\n";
        return;
    }
#endif
    glUniformMatrix3fv(l, 1, GL_FALSE, glm::value_ptr(m));
}

void Program::setMat4(const std::string& name, const glm::mat4& m) const {
    // Get location from map
    const GLint l = getUniformLocation(name);
    // In debug mode, check that location is valid
#ifndef NDEBUG
    if (l == -1) {
        std::cerr << "Invalid uniform location given\n";
        return;
    }
#endif
    glUniformMatrix4fv(l, 1, GL_FALSE, glm::value_ptr(m));
}

void Program::printUniforms() const {
    // Print uniform informations
    std::cout << "Program has " << m_uniforms_map.size() << " uniform/s\n";
    for (const auto& u : m_uniforms_map) {
        std::cout << "Name: " << u.first << " Location: " << u.second << "\n";
    }
}

void Program::printUniformBlocks() const {
    // Print uniform blocks informations
    std::cout << "Program has " << m_uniforms_block_map.size() << " uniform block/s\n";
    for (const auto& u : m_uniforms_block_map) {
        std::cout << "Uniform block name: " << u.first << "\n";
        u.second.printInformations();
    }
}

void Program::printAttributes() const {
    // Print attributes informations
    std::cout << "Program has " << m_attributes_map.size() << " attribute/s\n";
    for (const auto& a : m_attributes_map) {
        std::cout << "Name: " << a.first << " Location: " << a.second << "\n";
    }
}

void Program::printInformations() const {
    // Separator
    const std::string separator = "----------------------------------------------\n";

    std::cout << separator;
    printUniforms();
    printUniformBlocks();
    printAttributes();
    std::cout << separator;
}
