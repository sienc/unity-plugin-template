#pragma once

class UnityTexture;

#include "UnityPlugin.h"
#include "yup/FrameBuffer.h"

using yup::FrameBuffer;

class UnityTexture
{
private:
	const UnityPlugin & mUnityPlugin;
	const int mId;
	const void * mTexturePointer = nullptr;

	const int mWidth = 0;
	const int mHeight = 0;

public:

	UnityTexture(int id, void* texPtr, int width, int height, const UnityPlugin &plugin);
	~UnityTexture();

	void update(const FrameBuffer *buffer);

private:
	static void FillBuffer(unsigned char * dst, const FrameBuffer *buffer, int width, int height, int stride);
};
