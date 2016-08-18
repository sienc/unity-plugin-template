#pragma once

#ifdef YUP_INCLUDE_LIBREALSENSE

#include <memory>
#include <string>
#include <mutex>

// librealsense
#include <librealsense/rs.hpp>

#include "yup/LoopThread.h"
#include "yup/FrameBuffer.h"

using namespace yup;

class CamSource : LoopThread
{
	friend struct std::default_delete<CamSource>;

private:
	// Singleton instance
	static std::unique_ptr<CamSource> instance;

	std::atomic_bool mInitialized = false;
	std::atomic_bool mHasNewFrame = false;

	// Buffers
	FrameBuffer *mBuffer;
	FrameBuffer *mBackBuffer;

	std::mutex mFrameMutex;
	std::condition_variable mNewFrameCond;
	// librealsense
	rs::context ctx;
	rs::device * dev = nullptr;


private:
	CamSource();
	~CamSource();

	bool init() override;
	bool loop() override;
	void shutdown() override;

	const FrameBuffer *acquireFrame(bool wait = false);
	void releaseFrame();

	void swapBuffer();

public:
	// singleton implementation
	static CamSource & Instance() {
		if (!instance)
			instance = std::unique_ptr<CamSource>(new CamSource());

		return *instance;
	}

	static int Init() {
		Instance().run();
		return 0;
	}

	static void Shutdown() {
		if (instance)
			instance->stop();
	}

	static const FrameBuffer * AcquireFrame(bool wait = false) {
		if (instance)
			return instance->acquireFrame(wait);
		else
			return nullptr;
	}

	static void ReleaseFrame() {
		if (instance)
			instance->releaseFrame();
	}

private:
	// librealsense

};

#endif // YUP_INCLUDE_LIBREALSENSE