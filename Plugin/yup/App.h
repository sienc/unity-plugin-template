// ========================================================================== //
//
//  App.h
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

#include "yup.h"

BEGIN_NAMESPACE_YUP

class App
{
public:
	App(int argc, char *argv[]) {}
	virtual ~App() {}

	int exec();

	// virtual functions that needs overloading
protected:
	virtual bool init() { return true; }
	virtual bool update() = 0;
	virtual void shutdown() {}
};

END_NAMESPACE_YUP