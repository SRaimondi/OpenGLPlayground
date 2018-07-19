//
// Created by Simon on 17.07.18.
//

#include "GLUtils.hpp"
#include <string>
#include <iostream>

GLenum glCheckError(const char *file, int line) {
    GLenum error_code;
    while ((error_code = glGetError()) != GL_NO_ERROR) {
        std::string error;
        switch (error_code) {
            case GL_INVALID_ENUM:
                error = "INVALID_ENUM";
                break;
            case GL_INVALID_VALUE:
                error = "INVALID_VALUE";
                break;
            case GL_INVALID_OPERATION:
                error = "INVALID_OPERATION";
                break;
            case GL_STACK_OVERFLOW:
                error = "STACK_OVERFLOW";
                break;
            case GL_STACK_UNDERFLOW:
                error = "STACK_UNDERFLOW";
                break;
            case GL_OUT_OF_MEMORY:
                error = "OUT_OF_MEMORY";
                break;
            case GL_INVALID_FRAMEBUFFER_OPERATION:
                error = "INVALID_FRAMEBUFFER_OPERATION";
                break;
            default:
                error = "UNKNOWN ERROR";
                break;
        }
        std::cerr << error << " | " << file << " (" << line << ")\n";
    }
    return error_code;
}

std::string GLTypeToString(GLenum type) {
    switch (type) {
        case GL_BOOL:
            return "bool";
        case GL_INT:
            return "int";
        case GL_FLOAT:
            return "float";
        case GL_FLOAT_VEC2:
            return "vec2";
        case GL_FLOAT_VEC3:
            return "vec3";
        case GL_FLOAT_VEC4:
            return "vec4";
        case GL_FLOAT_MAT2:
            return "mat2";
        case GL_FLOAT_MAT3:
            return "mat3";
        case GL_FLOAT_MAT4:
            return "mat4";
        case GL_SAMPLER_2D:
            return "sampler2D";
        case GL_SAMPLER_3D:
            return "sampler3D";
        case GL_SAMPLER_CUBE:
            return "samplerCube";
        case GL_SAMPLER_2D_SHADOW:
            return "sampler2DShadow";
        default:
            return "Unknown type";
    }
}

GLsizei GLTypeToSize(GLenum type) {

#define CASE_MACRO(gl_type, count, type) \
case (gl_type): return (count) * sizeof(type)

    switch (type) {
        // Floats
        CASE_MACRO(GL_FLOAT, 1, GLfloat);
        CASE_MACRO(GL_FLOAT_VEC2, 2, GLfloat);
        CASE_MACRO(GL_FLOAT_VEC3, 3, GLfloat);
        CASE_MACRO(GL_FLOAT_VEC4, 4, GLfloat);
            // Ints
        CASE_MACRO(GL_INT, 1, GLint);
        CASE_MACRO(GL_INT_VEC2, 2, GLint);
        CASE_MACRO(GL_INT_VEC3, 3, GLint);
        CASE_MACRO(GL_INT_VEC4, 4, GLint);
            // Unsigned ints
        CASE_MACRO(GL_UNSIGNED_INT, 1, GLuint);
        CASE_MACRO(GL_UNSIGNED_INT_VEC2, 2, GLuint);
        CASE_MACRO(GL_UNSIGNED_INT_VEC3, 3, GLuint);
        CASE_MACRO(GL_UNSIGNED_INT_VEC4, 4, GLuint);
            // Bools
        CASE_MACRO(GL_BOOL, 1, GLboolean);
        CASE_MACRO(GL_BOOL_VEC2, 2, GLboolean);
        CASE_MACRO(GL_BOOL_VEC3, 3, GLboolean);
        CASE_MACRO(GL_BOOL_VEC4, 4, GLboolean);
            // Matrices
        CASE_MACRO(GL_FLOAT_MAT2, 4, GLfloat);
        CASE_MACRO(GL_FLOAT_MAT2x3, 6, GLfloat);
        CASE_MACRO(GL_FLOAT_MAT2x4, 8, GLfloat);
        CASE_MACRO(GL_FLOAT_MAT3, 9, GLfloat);
        CASE_MACRO(GL_FLOAT_MAT3x2, 6, GLfloat);
        CASE_MACRO(GL_FLOAT_MAT3x4, 12, GLfloat);
        CASE_MACRO(GL_FLOAT_MAT4, 16, GLfloat);
        CASE_MACRO(GL_FLOAT_MAT4x2, 8, GLfloat);
        CASE_MACRO(GL_FLOAT_MAT4x3, 12, GLfloat);

#undef CASE_MACRO

        default: {
            std::cerr << "Unkown type\n";
            exit(EXIT_FAILURE);
        }
    }

}
