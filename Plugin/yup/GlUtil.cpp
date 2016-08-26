// ========================================================================== //
//
//  GlUtil.cpp
//  ---
//
//  Created: 2016-08-24
//  Updated: 2016-08-24
//
//  (C) 2016 Yu-hsien Chang
//
// ========================================================================== //

#ifdef YUP_INCLUDE_GLEW

#include "GlUtil.h"
#include "ShaderCollection.h"
#include "Log.h"

#include <stdio.h>

BEGIN_NAMESPACE_YUP_GL

//-----------------------------------------------------------------------------
// Purpose: Compiles a GL shader program and returns the handle. Returns 0 if
//			the shader couldn't be compiled for some reason.
//-----------------------------------------------------------------------------
GLuint CompileGLShader(const char *pchShaderName, const char *pchVertexShader, const char *pchFragmentShader)
{
	GLuint unProgramID = glCreateProgram();

	GLuint nSceneVertexShader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(nSceneVertexShader, 1, &pchVertexShader, NULL);
	glCompileShader(nSceneVertexShader);

	GLint vShaderCompiled = GL_FALSE;
	glGetShaderiv(nSceneVertexShader, GL_COMPILE_STATUS, &vShaderCompiled);
	if (vShaderCompiled != GL_TRUE)
	{
		printf("%s - Unable to compile vertex shader %d!\n", pchShaderName, nSceneVertexShader);
		glDeleteProgram(unProgramID);
		glDeleteShader(nSceneVertexShader);
		return 0;
	}
	glAttachShader(unProgramID, nSceneVertexShader);
	glDeleteShader(nSceneVertexShader); // the program hangs onto this once it's attached

	GLuint  nSceneFragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(nSceneFragmentShader, 1, &pchFragmentShader, NULL);
	glCompileShader(nSceneFragmentShader);

	GLint fShaderCompiled = GL_FALSE;
	glGetShaderiv(nSceneFragmentShader, GL_COMPILE_STATUS, &fShaderCompiled);
	if (fShaderCompiled != GL_TRUE)
	{
		printf("%s - Unable to compile fragment shader %d!\n", pchShaderName, nSceneFragmentShader);
		glDeleteProgram(unProgramID);
		glDeleteShader(nSceneFragmentShader);
		return 0;
	}

	glAttachShader(unProgramID, nSceneFragmentShader);
	glDeleteShader(nSceneFragmentShader); // the program hangs onto this once it's attached

	glLinkProgram(unProgramID);

	GLint programSuccess = GL_TRUE;
	glGetProgramiv(unProgramID, GL_LINK_STATUS, &programSuccess);
	if (programSuccess != GL_TRUE)
	{
		printf("%s - Error linking program %d!\n", pchShaderName, unProgramID);
		glDeleteProgram(unProgramID);
		return 0;
	}

	glUseProgram(unProgramID);
	glUseProgram(0);

	return unProgramID;
}


////////////////////////////////////////////////////////////////////////////////
// - OpenGL utility functions
////////////////////////////////////////////////////////////////////////////////

bool GenTexture(GLuint *texId, GLsizei width, GLsizei height, GLint internalformat, GLenum format, GLenum type, const void *pixels)
{
	GlDumpError("GenTexture: error queue clean up");

	// setup texture
	glGenTextures(1, texId);

	if (*texId == 0)
	{
		GlDumpError("GenTexture: Unable to create texture");
		return false;
	}

	glActiveTexture(TEXTURE_RGB);
	glBindTexture(GL_TEXTURE_2D, *texId);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAX_LEVEL, 0);

	glTexImage2D(GL_TEXTURE_2D, 0, internalformat, width, height, 0, format, type, pixels);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);

	GLfloat fLargest;
	glGetFloatv(GL_MAX_TEXTURE_MAX_ANISOTROPY_EXT, &fLargest);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAX_ANISOTROPY_EXT, fLargest);

	glBindTexture(GL_TEXTURE_2D, 0);

	return GlDumpError("GenTexture");
}

// * Generate an OpenGL buffer of specified size
bool GenBuffer(GLuint &bufferID, size_t size)
{
	GlDumpError("GenBuffer: error queue clean up");

	glGenBuffers(1, &bufferID);

	if (bufferID <= 0)
	{
		GlDumpError("GenBuffer: Error generating PBO");
		return false;
	}

	glBindBuffer(GL_PIXEL_UNPACK_BUFFER, bufferID);
	glBufferData(GL_PIXEL_UNPACK_BUFFER, size, NULL, GL_STREAM_DRAW);
	glBindBuffer(GL_PIXEL_UNPACK_BUFFER, 0);

	return GlDumpError("GenBuffer: glBufferData()");
}

// * Update an OpenGL texture with a uploaded buffer
void UpdateTexture(GLuint texId, int width, int height, GLenum format, GLenum type, GLuint bufferId)
{
	glBindBuffer(GL_PIXEL_UNPACK_BUFFER, bufferId);

	glBindTexture(GL_TEXTURE_2D, texId);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAX_LEVEL, 0);
	glPixelStorei(GL_UNPACK_ROW_LENGTH, width);

	glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, width, height, format, type, 0);

	glBindTexture(GL_TEXTURE_2D, 0);

	glBindBuffer(GL_PIXEL_UNPACK_BUFFER, 0);
}

// * Update an OpenGL texture with a CPU pixel buffer
void UpdateTexture(GLuint texId, int width, int height, GLenum format, GLenum type, const void *pixels)
{
	glBindBuffer(GL_PIXEL_UNPACK_BUFFER, 0);

	glBindTexture(GL_TEXTURE_2D, texId);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAX_LEVEL, 0);
	glPixelStorei(GL_UNPACK_ROW_LENGTH, width);

	glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, width, height, format, type, pixels);

	glBindTexture(GL_TEXTURE_2D, 0);
}


bool GlDumpError(const std::string &msg)
{
	GLenum error = glGetError();
	bool ret = true;

	while (error != GL_NO_ERROR)
	{
//		std::string errorString = (char *)gluErrorString(error);
//		LogE("%s (GL_ERROR %d: %s)",msg.c_str(), error, errorString.c_str());

		ret = false;
		error = glGetError();
	}

	return ret;
}

END_NAMESPACE_YUP_GL

#endif // YUP_INCLUDE_GLEW
