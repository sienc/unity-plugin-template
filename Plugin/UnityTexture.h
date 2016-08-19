#pragma once

class UnityTexture;

#include "UnityPlugin.h"
#include "yup/FrameBuffer.h"

using yup::FrameBuffer;

struct TextureData
{
	void * mTexturePointer = nullptr;
	int mWidth = 0;
	int mHeight = 0;
};

class UnityTexture
{
private:
	void * mTexturePointer = nullptr;

	int mWidth = 0;
	int mHeight = 0;

	bool mIsOwned = false;

public:

	UnityTexture();
	UnityTexture(void * texPtr, int width, int height);
	~UnityTexture();

	bool create(int width, int height);
	void release();

	void assign(void * texPtr, int width, int height);

	void update(const FrameBuffer *buffer);

	bool isValid() const { return mTexturePointer != nullptr; }

	TextureData toStruct() const {
		TextureData data;
		data.mTexturePointer = this->mTexturePointer;
		data.mWidth = this->mWidth;
		data.mHeight = this->mHeight;

		return data;
	}

private:
	static void FillBuffer(unsigned char * dst, const FrameBuffer *buffer, int width, int height, int stride);
};
