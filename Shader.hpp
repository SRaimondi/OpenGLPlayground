//
// Created by Simon on 17.07.18.
//

#ifndef OPENGLPLAYGROUND_SHADER_HPP
#define OPENGLPLAYGROUND_SHADER_HPP

#include "GLUtils.hpp"
// GLM include
#define GLM_FORCE_RADIANS

#include <glm/glm.hpp>
// STL include
#include <unordered_map>
#include <vector>

// Shader types
enum class ShaderType : GLenum {
    Vertex = GL_VERTEX_SHADER,
    Fragment = GL_FRAGMENT_SHADER,
    TessControl = GL_TESS_CONTROL_SHADER,
    TessEvaluation = GL_TESS_EVALUATION_SHADER,
    Geometry = GL_GEOMETRY_SHADER
};

// Convert ShaderType to string
std::string shaderTypeToString(const ShaderType& type);

// Shader class, only wraps the shader part, not the program
class Shader {
private:
    // Shader ID
    GLuint m_shader_id;
    // Shader type
    const ShaderType m_type;

    // Compile shader
    void compile() const;

    // Print shader compilation log
    void printShaderLog() const;

public:
    // Constructor from a given string
    Shader(const std::string& file_name, const ShaderType& type);

    // Construct from a given list of strings, assumes the strings are null terminated
    explicit Shader(const std::vector<std::string>& sources, const ShaderType& type);

    // Destructor deletes the shader
    ~Shader();

    // Get id
    inline GLuint getID() const noexcept {
        return m_shader_id;
    }

    // Get type
    inline ShaderType getType() const noexcept {
        return m_type;
    }
};

// Program class, wraps the program OpenGL concept
class Program {
private:
    // Program ID
    GLuint m_program_id;

    // Unordered map containing the uniforms (name and location)
    mutable std::unordered_map<std::string, GLuint> m_uniforms_map;

    // Unordered map containing the uniforms block locations
    mutable std::unordered_map<std::string, GLuint> m_uniforms_block_map;

    // Unordered map containing the attributes (name and location)
    mutable std::unordered_map<std::string, GLuint> m_attributes_map;

    // Link program
    void link() const;

    // Pring program linking log
    void printProgramInfoLog() const;

public:
    // Construct shader from a given list of shaders
    Program(const std::initializer_list<Shader>& shaders);

    ~Program();

    // Use program
    inline void use() const noexcept {
        glUseProgram(m_program_id);
        GL_CHECK();
    }

    // Validate program
    void validate() const noexcept;

    // Get number of attached shaders
    GLint getNumAttachedShaders() const noexcept;

    // Prefetch a uniform location, returns false if the uniform was not found
    bool prefetchUniform(const std::string& uniform_name) const;

    bool prefetchUniforms(const std::initializer_list<std::string>& uniform_names) const;

    // Prefetch a uniform block location, returns false if the block is not found
    bool prefetchUniformBlock(const std::string& uniform_block_name) const;

    bool prefetchUniformBlocks(const std::initializer_list<std::string>& uniform_blocks_names) const;

    // Prefetch an attribute location, returns false if the attribute is not found
    bool prefetchAttribute(const std::string& attribute_name) const;

    bool prefetchAttributes(const std::initializer_list<std::string>& attribute_names) const;

    // Get location of a given uniform by name
    GLuint getUniformLocation(const std::string& uniform_name) const;

    // Get location of a given uniform block by name
    GLuint getUniformBlockLocation(const std::string& uniform_block_name) const;

    // Get location of a given attribute by name
    GLuint getAttributeLocation(const std::string& attrib_name) const;

    // Set values in the program using uniforms
    void setBool(const std::string& name, bool value) const;

    void setInt(const std::string& name, int value) const;

    void setFloat(const std::string& name, float value) const;

    void setVec2(const std::string& name, const glm::vec2& v) const;

    void setVec2(const std::string& name, float x, float y) const;

    void setVec3(const std::string& name, const glm::vec3& v) const;

    void setVec3(const std::string& name, float x, float y, float z) const;

    void setVec4(const std::string& name, const glm::vec4& v) const;

    void setVec4(const std::string& name, float x, float y, float z, float w) const;

    void setMat2(const std::string& name, const glm::mat2& m) const;

    void setMat3(const std::string& name, const glm::mat3& m) const;

    void setMat4(const std::string& name, const glm::mat4& m) const;

    // Print uniforms information
    void printUniforms() const;

    // Print uniforms blocks information
    void printUniformBlocks() const;

    // Print attributes information
    void printAttributes() const;

    // Print Program informations
    void printInformations() const;
};

#endif //OPENGLPLAYGROUND_SHADER_HPP
