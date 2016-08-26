// ========================================================================== //
//
//  LoopThread.h
//  ---
//  The native app class
//
//  Created: 2016-08-24
//  Updated: 2016-08-24
//
//  (C) 2016 Yu-hsien Chang
//
// ========================================================================== //

#pragma once

#include <atomic>
#include "yup.h"
#include "Thread.h"

BEGIN_NAMESPACE_YUP

// -------------------------------------------------------------------------- //
//  - Call run() to spawn the thread
//  - Call stop() to join the thread
// -------------------------------------------------------------------------- //
class LoopThread :
	public Thread
{
private:
	std::atomic_bool mStop = false;

public:
	LoopThread() {}
	virtual ~LoopThread() { stop(); }

	void stop() {
		mStop = true;
		onStop();
		join();
	}

	bool stopping() { return mStop; }

protected:
	virtual bool init() = 0;
	virtual bool loop() = 0;
	virtual void shutdown() = 0;

	virtual void onStop() {}

	virtual void threadFunc() {
		mStop = false;

		if (init())
			while (!mStop && loop());

		shutdown();
	}
};

END_NAMESPACE_YUP