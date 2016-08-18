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