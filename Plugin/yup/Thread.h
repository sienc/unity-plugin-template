// ========================================================================== //
//
//  Thread.h
//  ---
//  A thread class
//
//  Created: 2016-08-24
//  Updated: 2016-08-24
//
//  (C) 2016 Yu-hsien Chang
//
// ========================================================================== //

#pragma once

#include <thread>
#include "yup.h"

BEGIN_NAMESPACE_YUP
	
// Thread class
class Thread
{
	friend void ThreadFunc(Thread *thread);

private:
	std::thread * mThread = nullptr;

public:
	Thread() {}
	virtual ~Thread() { join(); }

	void run() {
		if (!mThread)
			mThread = new std::thread(ThreadFunc, this);
	}

	void join() {
		if (mThread)
		{
			if (mThread->joinable())
				mThread->join();

			delete mThread;
			mThread = nullptr;
		}
	}

protected:
	virtual void threadFunc() = 0;
};


// Thread function
static void ThreadFunc(yup::Thread *thread) {
	thread->threadFunc();
}


END_NAMESPACE_YUP
