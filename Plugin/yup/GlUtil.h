#pragma once

#ifdef YUP_INCLUDE_GLEW

#include <string>

#include "yup.h"
#include "incl_sdl.h"

BEGIN_NAMESPACE_YUP_GL

GLuint CompileGLShader(const char *pchShaderName, const char *pchVertexShader, const char *pchFragmentShader);

// OpenGL utility functions
bool GenTexture(GLuint *texId, GLsizei width, GLsizei height, GLint internalformat, GLenum format, GLenum type, const void *pixels = NULL);
bool GenBuffer(GLuint &bufferID, size_t size);
void UpdateTexture(GLuint texId, int width, int height, GLenum format, GLenum type, GLuint bufferId);
void UpdateTexture(GLuint texId, int width, int height, GLenum format, GLenum type, const void *pixels = NULL);

bool GlDumpError(const std::string &msg);

END_NAMESPACE_YUP_GL

#endif // YUP_INCLUDE_GLEW
