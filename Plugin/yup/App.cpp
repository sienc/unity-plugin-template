// ========================================================================== //
//
//  App.cpp
//  ---
//
//  Created: 2016-08-24
//  Updated: 2016-08-24
//
//  (C) 2016 Yu-hsien Chang
//
// ========================================================================== //

#include "App.h"

BEGIN_NAMESPACE_YUP

int App::exec()
{
	if (!init())
	{
		shutdown();
		return 1;
	}

	while (update());

	shutdown();

	return 0;
}

END_NAMESPACE_YUP