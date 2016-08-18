#ifdef YUP_INCLUDE_LIBREALSENSE

#include "CamSource.h"

#include <vector>

std::unique_ptr<CamSource> CamSource::instance = nullptr;

CamSource::CamSource()
{
	mBuffer = new FrameBuffer(3, 640, 480);
	mBackBuffer = new FrameBuffer(3, 640, 480);
}


CamSource::~CamSource()
{
	if (mInitialized)
		shutdown();

	if (mBuffer)
	{
		delete mBuffer;
		mBuffer = nullptr;
	}

	if (mBackBuffer)
	{
		delete mBackBuffer;
		mBackBuffer = nullptr;
	}
}

bool CamSource::init()
{
	if (mInitialized)
		return true;

	if (ctx.get_device_count() == 0)
		return false;

	dev = ctx.get_device(0);

//	dev->enable_stream(rs::stream::depth, 640, 480, rs::format::z16, 30);
	dev->enable_stream(rs::stream::color, 640, 480, rs::format::rgb8, 30);

	dev->start();

	mInitialized = true;

	return true;

}

// Update the frame buffer
// Returns true if the frame is successfully updated
bool CamSource::loop()
{
	if (!mInitialized)
		return false;

	dev->wait_for_frames();

	mBackBuffer->setData((const uint8_t *)dev->get_frame_data(rs::stream::color));

	swapBuffer();

	return true;
}


void CamSource::shutdown()
{
	mInitialized = false;

	if (dev)
	{
		dev->stop();

		dev = nullptr;
	}
}

const FrameBuffer * CamSource::acquireFrame(bool wait)
{
	if (!mInitialized)
		return nullptr;

	if(!mHasNewFrame && !wait)
		return nullptr;

	//if (!mHasNewFrame)
	//{
	//	std::unique_lock<std::mutex> lk(mFrameMutex);
	//	mNewFrameCond.wait(lk);
	//}


//	if (mInitialized && mHasNewFrame)
	{
		mFrameMutex.lock();
		mHasNewFrame = false;
		return mBuffer;
	}

	return nullptr;
}

void CamSource::releaseFrame()
{
	mFrameMutex.unlock();
}

void CamSource::swapBuffer()
{
	{
		std::lock_guard<std::mutex> lock(mFrameMutex);

		FrameBuffer * tmpBuffer = mBuffer;
		mBuffer = mBackBuffer;
		mBackBuffer = tmpBuffer;

		mHasNewFrame = true;
	}

	mNewFrameCond.notify_all();
}

#endif // YUP_INCLUDE_LIBREALSENSE