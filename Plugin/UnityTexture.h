#pragma once

class UnityTexture;

#include "UnityPlugin.h"
#include "yup/FrameBuffer.h"

using yup::FrameBuffer;

extern "C"
{
	struct TextureData
	{
		void * mResViewPointer;
		int mWidth;
		int mHeight;
	};
}

class UnityTexture
{
private:
	void * mTexturePointer = nullptr;
	void * mResViewPointer = nullptr;

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

	void write(const FrameBuffer * buffer);

	bool isValid() const { return mTexturePointer != nullptr; }

	TextureData toStruct() const {
		TextureData desc;
		desc.mResViewPointer = this->mResViewPointer;
		desc.mWidth = this->mWidth;
		desc.mHeight = this->mHeight;

		return desc;
	}

private:
	static void FillBuffer(unsigned char * dst, const FrameBuffer *buffer, int width, int height, int stride);
};
