// ========================================================================== //
//
//  Singleton.h
//  ---
//  Singlton implementation example
//
//  Created: 2016-08-24
//  Updated: 2016-08-24
//
//  (C) 2016 Yu-hsien Chang
//
// ========================================================================== //

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