// ========================================================================== //
//
//  ShaderCollection.h
//  ---
//  GL shader wrappers
//
//  Created: 2016-08-24
//  Updated: 2016-08-24
//
//  (C) 2016 Yu-hsien Chang
//
// ========================================================================== //

#pragma once

#ifdef YUP_INCLUDE_GLEW

#include <vector>
#include <string>

#include "yup.h"
#include "inc_sdl.h"
#include "ShaderSource.h"
#include "GlUtil.h"
#include "Matrices.h"

#define TEXTURE_RGB			GL_TEXTURE0
#define TEXTURE_YUV_Y		GL_TEXTURE0
#define TEXTURE_YUV_U		GL_TEXTURE1
#define TEXTURE_YUV_V		GL_TEXTURE2
#define TEXTURE_DEPTH		GL_TEXTURE3

#define UVMODE_FULL					0
#define UVMODE_TOP_HALF				1
#define UVMODE_BOTTOM_HALF			2
#define UVMODE_LEFT_HALF			3
#define UVMODE_RIGHT_HALF			4
#define UVMODE_RGB_OVER_DEPTH		5

#define COLORMODE_RGB				0
#define COLORMODE_YUV				1

#define DEPTHMODE_NONE				0
#define DEPTHMODE_LINEAR			1
#define DEPTHMODE_REVERSED_LINEAR	2
#define DEPTHMODE_INVERSE			3

BEGIN_NAMESPACE_YUP_GL

//============================================================================//
//
// GL Uniform Wrappers
//
//============================================================================//

struct GlUniformLoc
{
	GLint loc = 0;
};

struct GlUniformInt : public GlUniformLoc
{
	void operator=(int val) {
		glUniform1i(loc, val);
	}
};

struct GlUniformFloat : public GlUniformLoc
{
	void operator=(float val) {
		glUniform1f(loc, val);
	}
};

struct GlUniformVec4 : public GlUniformLoc
{
	void operator=(const Vector4 &color) {
		glUniform4f(loc, color.x, color.y, color.z, color.w);
	}
};

struct GlUniformMat4 : public GlUniformLoc
{
	void operator=(const Matrix4 & mat) {
		glUniformMatrix4fv(loc, 1, GL_FALSE, mat.get());
	}
};


//============================================================================//
//
// Shader Programs
//
//============================================================================//

class ShaderProgram
{
public:
	GLuint Id = 0;

private:
	std::string mName;
	std::string mVertShader;
	std::string mFragShader;

public:
	ShaderProgram(const char *name, const char *vertShader, const char *fragShader) {
		mName = name;
		mVertShader = vertShader;
		mFragShader = fragShader;
	}

	bool compile() {
		Id = CompileGLShader(mName.c_str(), mVertShader.c_str(), mFragShader.c_str());
		mVertShader.clear();
		mFragShader.clear();
		
		if (Id == 0)
			return false;

		use();
		onCompile();
		unuse();

		return true;
	}

	void cleanup() {
		if (Id)
		{
			onCleanup();
			glDeleteProgram(Id);
			Id = 0;
		}
	}

	GLint getLoc(const char *uniformName) {
		return glGetUniformLocation(Id, uniformName);
	}

	void use() {
		glUseProgram(Id);
	}

	void unuse() {
		glUseProgram(0);
	}

	virtual void onCompile() {};
	virtual void onCleanup() {};

};

//----------------------------------------------------------------------------//
// Solid color
//----------------------------------------------------------------------------//
class SolidProgram : public ShaderProgram
{
public:
	GlUniformMat4 MvpMatrix;
	GlUniformVec4 Color;

public:
	SolidProgram() : ShaderProgram("Solid", SIMPLE_VERTEX_SHADER, SOLID_FRAGMENT_SHADER) {}

	virtual void onCompile() override {
		MvpMatrix.loc = getLoc("matrix");
		Color.loc = getLoc("color");
	}
};


//----------------------------------------------------------------------------//
// Solid color
//----------------------------------------------------------------------------//
class VColorProgram : public ShaderProgram
{
public:
	GlUniformMat4 MvpMatrix;
	GlUniformVec4 Color;

public:
	VColorProgram() : ShaderProgram("VColor", VCOLOR_VERTEX_SHADER, VCOLOR_FRAGMENT_SHADER) {}

	virtual void onCompile() override {
		MvpMatrix.loc = getLoc("matrix");
	}
};


//----------------------------------------------------------------------------//
// Color based on world position
//----------------------------------------------------------------------------//
class PosProgram : public ShaderProgram
{
public:
	GlUniformMat4 MvpMatrix;

public:
	PosProgram() : ShaderProgram("Pos", POS_VERTEX_SHADER, POS_FRAGMENT_SHADER) {}

	virtual void onCompile() override {
		MvpMatrix.loc = getLoc("matrix");
	}
};

//----------------------------------------------------------------------------//
// Color based on z-depth
//----------------------------------------------------------------------------//
class DepthProgram : public ShaderProgram
{
public:
	GlUniformMat4 MvpMatrix;

public:
	DepthProgram() : ShaderProgram("Depth", SIMPLE_VERTEX_SHADER, DEPTH_FRAGMENT_SHADER) {}

	virtual void onCompile() override {
		MvpMatrix.loc = getLoc("matrix");
	}
};

//----------------------------------------------------------------------------//
// Maps a mono RGB texture
//----------------------------------------------------------------------------//
class RgbProgram : public ShaderProgram
{
public:
	GlUniformMat4 MvpMatrix;

public:
	RgbProgram() : ShaderProgram("Rgb", UV_VERTEX_SHADER, RGB_FRAGMENT_SHADER) {}

	virtual void onCompile() override {
		MvpMatrix.loc = getLoc("matrix");

		glUniform1i(getLoc("tex0"), 0);
	}
};

//----------------------------------------------------------------------------//
// Maps a mono resizable RGB texture
//----------------------------------------------------------------------------//
class MonoResizableProgram : public ShaderProgram
{
public:
	GlUniformMat4 MvpMatrix;
	GlUniformFloat URatio;
	GlUniformFloat VRatio;

public:
	MonoResizableProgram() : ShaderProgram("MonoResize", MONO_RESIZABLE_VERTEX_SHADER, RGB_FRAGMENT_SHADER) {}

	virtual void onCompile() override {
		MvpMatrix.loc = getLoc("matrix");
		URatio.loc = getLoc("u_ratio");
		VRatio.loc = getLoc("v_ratio");
	}
};

//----------------------------------------------------------------------------//
// Maps a mono or stereo texture
// Texture can be RGB or HSV
//----------------------------------------------------------------------------//
class StereoProgram : public ShaderProgram
{
public:
	GlUniformMat4 MvpMatrix;
	GlUniformInt UVMode;
	GlUniformInt ColorMode;
	GlUniformFloat Opacity;
	GlUniformInt ClipOutOfBound;

public:
	StereoProgram() : ShaderProgram("Mono", STEREO_VERTEX_SHADER, STEREO_RGBHSV_FRAGMENT_SHADER) {}

	virtual void onCompile() override {

		glUniform1i(getLoc("tex0"), 0);
		glUniform1i(getLoc("tex1"), 1);
		glUniform1i(getLoc("tex2"), 2);
		
		MvpMatrix.loc = getLoc("matrix");
		UVMode.loc = getLoc("uv_mode");
		ColorMode.loc = getLoc("color_mode");
		Opacity.loc = getLoc("opacity");
		ClipOutOfBound.loc = getLoc("clip_out_of_bound");
	}
};


//============================================================================//
//
// Shader Collection
//
//============================================================================//

class ShaderCollection
{
private:
	std::vector<ShaderProgram *> mPrograms;

public:
	SolidProgram Solid;
	VColorProgram VColor;
	PosProgram Pos;
	DepthProgram Depth;
	//MonoProgram Mono;
	//MonoResizableProgram MonoResizable;
	//StereoProgram Stereo;

public:
	ShaderCollection() {
		mPrograms.push_back(&Solid);
		mPrograms.push_back(&VColor);
		mPrograms.push_back(&Pos);
		mPrograms.push_back(&Depth);
	}

	bool Compile() {
		for (ShaderProgram *program : mPrograms)
		{
			if (!program->compile())
				return false;
		}
		return true;
	}

	void Cleanup() {
		for (ShaderProgram *program : mPrograms)
			program->cleanup();
	}
};

extern ShaderCollection Shader;

END_NAMESPACE_YUP_GL

#endif // YUP_INCLUDE_GLEW