// ========================================================================== //
//
//  glutil.h
//  ---
//  Utility functions for Handling GL textures and buffers
//
//  Created: 2016-08-24
//  Updated: 2016-08-24
//
//  (C) 2016 Yu-hsien Chang
//
// ========================================================================== //

#pragma once

#ifdef YUP_INCLUDE_GLEW

#include <string>

#include "yup.h"
#include "inc_sdl.h"

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
