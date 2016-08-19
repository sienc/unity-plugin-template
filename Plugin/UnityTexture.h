#pragma once

class UnityTexture;

#include "yup/FrameBuffer.h"

using yup::FrameBuffer;

extern "C"
{
	typedef struct _TexturePacket
	{
		void * mTexturePointer;
		int mWidth;
		int mHeight;
	} TexturePacket;
}

class UnityTexture
{
private:
	void * mTexturePointer = nullptr;
	void * mExternalPointer = nullptr;

	int mWidth = 0;
	int mHeight = 0;

	bool mIsOwned = false;

public:

	UnityTexture();
	UnityTexture(void * texPtr, int width, int height);
	~UnityTexture();

	bool create(int width, int height);
	void release();

	void set(void * texPtr, int width, int height);

	void write(const FrameBuffer * buffer);

	bool isValid() const { return mTexturePointer != nullptr; }

	TexturePacket toPacket() const {
		return { mExternalPointer, mWidth, mHeight };
	}

private:
	static void FillBuffer(unsigned char * dst, const FrameBuffer *buffer, int width, int height, int stride);
};
