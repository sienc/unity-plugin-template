#pragma once

#include <cstdint>

#ifndef PTRINT
#ifdef WIN32
#define PTRINT uint32_t
#else
#define PTRINT uint64_t
#endif
#endif

#define PTR2UINT(p) ((GLuint)(PTRINT)p)
#define UINT2PTR(n) ((void *)(PTRINT)n)


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

	void write(const uint8_t * buffer, int width, int height, int stride);

	bool isValid() const { return mTexturePointer != nullptr; }

	TexturePacket toPacket() const {
		return { mExternalPointer, mWidth, mHeight };
	}

private:
	static void FillBuffer(unsigned char * dst, const uint8_t *buffer, int width, int height, int stride);
};
