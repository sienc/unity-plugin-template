#pragma once

#include "yup.h"

BEGIN_NAMESPACE_YUP

class Singleton
{
private:
	Singleton() {}

public:
	// singleton implementation
	static Singleton & Instance() {
		static Singleton instance;

		return instance;
	}

	Singleton(const Singleton &) = delete;
	void operator=(const Singleton &) = delete;
};

END_NAMESPACE_YUP