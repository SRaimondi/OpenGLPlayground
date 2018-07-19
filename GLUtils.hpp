//
// Created by Simon on 17.07.18.
//

#ifndef OPENGLPLAYGROUND_GLUTILS_HPP
#define OPENGLPLAYGROUND_GLUTILS_HPP

#include <GL/glew.h>
#include <string>

// Check OpenGL errors
GLenum glCheckError(const char *file, int line);

#ifndef NDEBUG
#define GL_CHECK() glCheckError(__FILE__, __LINE__)
#else
#define GL_CHECK()
#endif

// Convert type to string
std::string GLTypeToString(GLenum type);

// Convert type to size
std::size_t GLTypeToSize(GLenum type);

#endif //OPENGLPLAYGROUND_GLUTILS_HPP
