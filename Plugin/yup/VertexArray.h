// ========================================================================== //
//
//  VertexArray.h
//  ---
//  GL vertex array
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
#include <mutex>

#include "yup.h"
#include "inc_sdl.h"
#include "Matrices.h"

#ifdef YUP_INCLUDE_OPENCV
#include <opencv2/core.hpp>
#endif // YUP_INCLUDE_OPENCV

BEGIN_NAMESPACE_YUP_GL

class VertexArray
{
private:
	GLuint mVAO = 0;
	GLuint mVertBufferId = 0;

	const GLsizei mStride0 = 0;
	const GLsizei mStride1 = 0;
	const GLsizei mStride2 = 0;

protected:
	std::vector<float> mVertArray;
	bool mVertArrayChanged = false;
	GLsizei mVertCount = 0;
	GLsizei mUploadedVertCount = 0;

	std::mutex mMutex;

public:
	//VertexArray();
	VertexArray(int stride1, int stride2 = 0, int stride3 = 0);
	virtual ~VertexArray();

	virtual bool init();
	virtual void update();
	virtual void shutdown();

	inline void bind() { glBindVertexArray(mVAO); }
	inline void unbind() { glBindVertexArray(0); }

	inline void draw(GLenum mode) {
		std::lock_guard<std::mutex> lock(mMutex);
		glBindVertexArray(mVAO);
		glDrawArrays(mode, 0, mUploadedVertCount);
		glBindVertexArray(0);
	}

	inline void drawPoints() { draw(GL_POINTS); }
	inline void drawTriangles() { draw(GL_TRIANGLES); }

	void clear() { mVertArray.clear(); mVertCount = 0; mVertArrayChanged = true; }

private:
	void updateVertexBuffer();
};

class SimpleVertexArray : public VertexArray
{
public:
	SimpleVertexArray() : VertexArray(3) {}

	void addPoint(float x, float y, float z);
#ifdef YUP_INCLUDE_OPENCV
	void addPoint(const cv::Point3f &pt) { addPoint(pt.x, pt.y, pt.z); }
#endif // YUP_INCLUDE_OPENCV
};

class UvVertexArray : public VertexArray
{
public:
	UvVertexArray() : VertexArray(3, 2) {}

	void addPoint(float x, float y, float z, float u = 0, float v = 0);
};

class RgbVertexArray : public VertexArray
{
public:
	RgbVertexArray() : VertexArray(3, 4) {}

	void addPoint(float x, float y, float z, float r = 1.0f, float g = 1.0f, float b = 1.0f, float a = 1.0f);
#ifdef YUP_INCLUDE_OPENCV
	void addPoint(const cv::Point3f &pt, const Vector4 &color) { addPoint(pt.x, pt.y, pt.z, color.x, color.y, color.z, color.w); }
#endif // YUP_INCLUDE_OPENCV
};

END_NAMESPACE_YUP_GL

#endif // YUP_INCLUDE_GLEW
