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

#define TYPE_TO_STRING(gl_type, type) \
case (gl_type): return #type

    switch (type) {
        // Floats
        TYPE_TO_STRING(GL_FLOAT, float);
        TYPE_TO_STRING(GL_FLOAT_VEC2, vec2);
        TYPE_TO_STRING(GL_FLOAT_VEC3, vec3);
        TYPE_TO_STRING(GL_FLOAT_VEC4, vec4);

            // Bool
        TYPE_TO_STRING(GL_BOOL, bool);

            // Int
        TYPE_TO_STRING(GL_INT, int);

            // Matrices
        TYPE_TO_STRING(GL_FLOAT_MAT2, mat2);
        TYPE_TO_STRING(GL_FLOAT_MAT3, mat3);
        TYPE_TO_STRING(GL_FLOAT_MAT4, mat4);

            // Samplers
        TYPE_TO_STRING(GL_SAMPLER_2D, sampler2D);
        TYPE_TO_STRING(GL_SAMPLER_3D, sampler3D);
        TYPE_TO_STRING(GL_SAMPLER_CUBE, samplerCube);
        TYPE_TO_STRING(GL_SAMPLER_2D_SHADOW, sampler2DShadow);

#undef TYPE_TO_STRING

        default:
            return "Unknown type";
    }

}

GLsizei GLTypeToSize(GLenum type) {

#define TYPE_TO_SIZE(gl_type, count, type) \
case (gl_type): return (count) * sizeof(type)

    switch (type) {
        // Floats
        TYPE_TO_SIZE(GL_FLOAT, 1, GLfloat);
        TYPE_TO_SIZE(GL_FLOAT_VEC2, 2, GLfloat);
        TYPE_TO_SIZE(GL_FLOAT_VEC3, 3, GLfloat);
        TYPE_TO_SIZE(GL_FLOAT_VEC4, 4, GLfloat);
            // Ints
        TYPE_TO_SIZE(GL_INT, 1, GLint);
        TYPE_TO_SIZE(GL_INT_VEC2, 2, GLint);
        TYPE_TO_SIZE(GL_INT_VEC3, 3, GLint);
        TYPE_TO_SIZE(GL_INT_VEC4, 4, GLint);
            // Unsigned ints
        TYPE_TO_SIZE(GL_UNSIGNED_INT, 1, GLuint);
        TYPE_TO_SIZE(GL_UNSIGNED_INT_VEC2, 2, GLuint);
        TYPE_TO_SIZE(GL_UNSIGNED_INT_VEC3, 3, GLuint);
        TYPE_TO_SIZE(GL_UNSIGNED_INT_VEC4, 4, GLuint);
            // Bools
        TYPE_TO_SIZE(GL_BOOL, 1, GLboolean);
        TYPE_TO_SIZE(GL_BOOL_VEC2, 2, GLboolean);
        TYPE_TO_SIZE(GL_BOOL_VEC3, 3, GLboolean);
        TYPE_TO_SIZE(GL_BOOL_VEC4, 4, GLboolean);
            // Matrices
        TYPE_TO_SIZE(GL_FLOAT_MAT2, 4, GLfloat);
        TYPE_TO_SIZE(GL_FLOAT_MAT2x3, 6, GLfloat);
        TYPE_TO_SIZE(GL_FLOAT_MAT2x4, 8, GLfloat);
        TYPE_TO_SIZE(GL_FLOAT_MAT3, 9, GLfloat);
        TYPE_TO_SIZE(GL_FLOAT_MAT3x2, 6, GLfloat);
        TYPE_TO_SIZE(GL_FLOAT_MAT3x4, 12, GLfloat);
        TYPE_TO_SIZE(GL_FLOAT_MAT4, 16, GLfloat);
        TYPE_TO_SIZE(GL_FLOAT_MAT4x2, 8, GLfloat);
        TYPE_TO_SIZE(GL_FLOAT_MAT4x3, 12, GLfloat);

#undef TYPE_TO_SIZE

        default: {
            std::cerr << "Unkown type\n";
            exit(EXIT_FAILURE);
        }
    }

}
