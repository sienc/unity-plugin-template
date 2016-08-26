// ========================================================================== //
//
//  VertexArray.cpp
//  ---
//
//  Created: 2016-08-24
//  Updated: 2016-08-24
//
//  (C) 2016 Yu-hsien Chang
//
// ========================================================================== //

#ifdef YUP_INCLUDE_GLEW

#include "VertexArray.h"

BEGIN_NAMESPACE_YUP_GL

VertexArray::VertexArray(int stride0, int stride1, int stride2)
	: mStride0(stride0)
	, mStride1(stride1)
	, mStride2(stride2)
{
}


VertexArray::~VertexArray()
{
}

bool VertexArray::init()
{
	// Generate VAO
	glGenVertexArrays(1, &mVAO);
	glBindVertexArray(mVAO);
	glGenBuffers(1, &mVertBufferId);
	
	//GLsizei stride = sizeof(float) * (mStride0 + mStride1 + mStride2);

	//uintptr_t offset = 0;
	//if (mStride0 > 0)
	//{
	//	glEnableVertexAttribArray(0);
	//	glVertexAttribPointer(0, mStride0, GL_FLOAT, GL_FALSE, stride, (const void *)offset);
	//	offset += sizeof(float) * mStride0;
	//}

	//if (mStride1 > 0)
	//{
	//	glEnableVertexAttribArray(1);
	//	glVertexAttribPointer(1, mStride1, GL_FLOAT, GL_FALSE, stride, (const void *)offset);
	//	offset += sizeof(float) * mStride1;
	//}

	//if (mStride2 > 0)
	//{
	//	glEnableVertexAttribArray(2);
	//	glVertexAttribPointer(2, mStride2, GL_FLOAT, GL_FALSE, stride, (const void *)offset);
	//}

	glBindVertexArray(0);

	updateVertexBuffer();

	return true;
}

void VertexArray::update()
{
	std::lock_guard<std::mutex> lock(mMutex);

	if (mVertArrayChanged)
		updateVertexBuffer();
}

void VertexArray::shutdown()
{
	if (mVertBufferId)
	{
		glDeleteBuffers(1, &mVertBufferId);
		mVertBufferId = 0;
	}

	if (mVAO)
	{
		glDeleteVertexArrays(1, &mVAO);
		mVAO = 0;
	}
}

void VertexArray::updateVertexBuffer()
{
	glBindVertexArray(mVAO);

	glBindBuffer(GL_ARRAY_BUFFER, mVertBufferId);
	if(mVertArray.size() > 0)
		glBufferData(GL_ARRAY_BUFFER, sizeof(float) * mVertArray.size(), &mVertArray[0], GL_STATIC_DRAW);
	else
		glBufferData(GL_ARRAY_BUFFER, 1, nullptr, GL_STATIC_DRAW);

	mUploadedVertCount = mVertCount;

	// VertexAttribPointer needs to be reassigned
	GLsizei stride = sizeof(float) * (mStride0 + mStride1 + mStride2);

	uintptr_t offset = 0;
	if (mStride0 > 0)
	{
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, mStride0, GL_FLOAT, GL_FALSE, stride, (const void *)offset);
		offset += sizeof(float) * mStride0;
	}

	if (mStride1 > 0)
	{
		glEnableVertexAttribArray(1);
		glVertexAttribPointer(1, mStride1, GL_FLOAT, GL_FALSE, stride, (const void *)offset);
		offset += sizeof(float) * mStride1;
	}

	if (mStride2 > 0)
	{
		glEnableVertexAttribArray(2);
		glVertexAttribPointer(2, mStride2, GL_FLOAT, GL_FALSE, stride, (const void *)offset);
	}

	glBindVertexArray(0);

	mVertArrayChanged = false;
}



void SimpleVertexArray::addPoint(float x, float y, float z)
{
	std::lock_guard<std::mutex> lock(mMutex);
	mVertArray.push_back(x);
	mVertArray.push_back(y);
	mVertArray.push_back(z);

	mVertCount += 1;
	mVertArrayChanged = true;
}


void UvVertexArray::addPoint(float x, float y, float z, float u, float v)
{
	std::lock_guard<std::mutex> lock(mMutex);

	mVertArray.push_back(x);
	mVertArray.push_back(y);
	mVertArray.push_back(z);
	mVertArray.push_back(u);
	mVertArray.push_back(v);

	mVertCount += 1;

	mVertArrayChanged = true;
}

void RgbVertexArray::addPoint(float x, float y, float z, float r, float g, float b, float a)
{
	std::lock_guard<std::mutex> lock(mMutex);

	mVertArray.push_back(x);
	mVertArray.push_back(y);
	mVertArray.push_back(z);
	mVertArray.push_back(r);
	mVertArray.push_back(g);
	mVertArray.push_back(b);
	mVertArray.push_back(a);

	mVertCount += 1;

	mVertArrayChanged = true;
}

END_NAMESPACE_YUP_GL

#endif // YUP_INCLUDE_GLEW