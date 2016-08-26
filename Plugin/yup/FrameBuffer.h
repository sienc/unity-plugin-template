// ========================================================================== //
//
//  FrameBuffer.h
//  ---
//  A generalized FrameBuffer class that automatically manages the storage
//
//  Created: 2016-08-24
//  Updated: 2016-08-24
//
//  (C) 2016 Yu-hsien Chang
//
// ========================================================================== //

#pragma once

#include <cstdint>
#include <cstring>

#ifdef YUP_INCLUDE_OPENCV
#include <opencv2/imgproc/imgproc.hpp>
#endif

#ifdef USE_RSSDK
#include <pxcsensemanager.h>
#endif

#include "yup.h"

BEGIN_NAMESPACE_YUP

#ifdef YUP_INCLUDE_OPENCV

static void LoadMatrix8(cv::Mat &mat, const uint8_t *data);
static void LoadMatrix16(cv::Mat &mat, const uint16_t *data);

#endif // YUP_INCLUDE_OPENCV

// A simple RGBA frame buffer
class FrameBuffer
{
private:
	uint8_t * mData = nullptr;

	int mWidth = 0;
	int mHeight = 0;
	int mColorDepth = 4;

public:
	FrameBuffer(int width = 0, int height = 0, int colorDepth = 4)
	: mColorDepth(colorDepth)
	{
		resize(width, height);
	}

	~FrameBuffer() {
		if (mData) {
			delete[] mData;
			mData = nullptr;
		}
	}

	int width() const { return mWidth; }
	int height() const { return mHeight; }
	int pitch() const { return mWidth * mColorDepth; }
	int size() const { return mWidth * mHeight * mColorDepth; } // Return the size of mdata

	uint8_t * data() { return mData; }
	const uint8_t * cst_data() const { return mData; }

	operator const uint8_t *() { return mData; }

	void resize(int width, int height, int colorDepth = 0) {
		if (colorDepth == 0)
			colorDepth = mColorDepth;

		if (mWidth * mHeight * mColorDepth != width * height * colorDepth)
		{
			if (mData)
				delete[] mData;

			size_t dataSize = width * height * mColorDepth;
			mData = new uint8_t[dataSize];
			memset(mData, 0, dataSize);
		}
		mWidth = width;
		mHeight = height;
		mColorDepth = colorDepth;
	}

	void clear() {
		if(mData)
			memset(mData, 0, size());
	}

	// Set the alpha channel to 0
	void clearMask() {

	}

	void setData(const uint8_t *data) {
		
		// Blit the image
		memcpy_s(mData, size(), data, size());
	}


	void setData(const uint8_t *colorData, const uint8_t *depthData) {

		uint8_t *dst = mData;

		// Expand for more efficiency

		if (mColorDepth >= 4)
		{
			// Copy both color and depth
			for (int i = 0; i<mHeight; i++)
			{
				for (int j = 0; j<mWidth; j++)
				{
					if (colorData)
					{
						dst[0] = *colorData++;
						dst[1] = *colorData++;
						dst[2] = *colorData++;
					}

					if (depthData)
						dst[3] = *depthData++;
					else
						dst[3] = 0xFF;

					dst += mColorDepth;
				}
			}
		}
		else if (mColorDepth == 3 && colorData)
		{
			// Copy only color
			setData(colorData);
		}
		else if (mColorDepth == 1 && depthData)
		{
			// Copy only depth
			setData(depthData);
		}
	}

	void operator=(const FrameBuffer & buffer)
	{
		resize(buffer.mWidth, buffer.mHeight, buffer.mColorDepth);
		memcpy_s(mData, size(), buffer.mData, size());
	}

#ifdef YUP_INCLUDE_OPENCV

	void setData(const cv::Mat &colorMat, const cv::Mat &depthMat, bool showDepth = false) {

		// accept only char type matrices
		CV_Assert(colorMat.depth() == CV_8U && colorMat.elemSize() == 3);
		CV_Assert(depthMat.depth() == CV_8U && depthMat.elemSize() == 1);
		CV_Assert(colorMat.rows == depthMat.rows && colorMat.cols == depthMat.cols);

		int width = colorMat.cols;
		int height = colorMat.rows;

		// Resize mBackBuffer to fit the image
		resize(width, height);

		if (colorMat.isContinuous() && depthMat.isContinuous())
		{
			width *= height;
			height = 1;
		}

		uint8_t *dst = mData;
		const uint8_t *pColor;
		const uint8_t *pDepth;
		for (int h = 0; h < height; ++h)
		{
			pColor = colorMat.ptr<uint8_t>(h);
			pDepth = depthMat.ptr<uint8_t>(h);
			for (int w = 0; w < width; ++w)
			{
				if (!showDepth)
				{
					dst[0] = pColor[w * 3];
					dst[1] = pColor[w * 3 + 1];
					dst[2] = pColor[w * 3 + 2];
					dst[3] = pDepth[w];
				}
				else
				{
					dst[0] = pDepth[w];
					dst[1] = pDepth[w];
					dst[2] = pDepth[w];
					dst[3] = pDepth[w];
				}

				dst += mColorDepth;
			}
		}
	}

	void loadMatrix(cv::Mat &mat) const {
		// accept only char type matrices
		CV_Assert(mat.rows == mHeight);
		CV_Assert(mat.cols == mWidth);
		
		LoadMatrix8(mat, mData);
	}

#endif // YUP_INCLUDE_OPENCV

};

#ifdef YUP_INCLUDE_OPENCV

static void LoadMatrix8(cv::Mat &mat, const uint8_t *data)
{
	// accept only char type matrices
	CV_Assert(mat.depth() == CV_8U);

	const int channels = mat.channels();

	int nRows = mat.rows;
	int nCols = mat.cols * channels;

	if (mat.isContinuous())
	{
		nCols *= nRows;
		nRows = 1;
	}


	uint8_t* p;
	for (int r = 0; r < nRows; ++r)
	{
		p = mat.ptr<uint8_t>(r);
		for (int c = 0; c < nCols; ++c)
			p[c] = data[c];

		data += nCols * channels;
	}
}

static void LoadMatrix16(cv::Mat &mat, const uint16_t *data)
{
	// accept only char type matrices
	CV_Assert(mat.depth() == CV_16U);

	const int channels = mat.channels();

	int nRows = mat.rows;
	int nCols = mat.cols * channels;

	if (mat.isContinuous())
	{
		nCols *= nRows;
		nRows = 1;
	}

	uint16_t* p;
	for (int r = 0; r < nRows; ++r)
	{
		p = mat.ptr<uint16_t>(r);
		for (int c = 0; c < nCols; ++c)
			p[c] = data[c];

		data += nCols * channels;
	}
}

#endif // YUP_INCLUDE_OPENCV

END_NAMESPACE_YUP