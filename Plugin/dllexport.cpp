#ifdef _USRDLL

#include "dllexport.h"

// -------------------------------------------------------------------------- //
// Specialized API Exports
// -------------------------------------------------------------------------- //

extern "C" int UNITY_INTERFACE_EXPORT UNITY_INTERFACE_API PluginFunc()
{
	return Plugin.PluginFunc();
}

#endif _USRDLL