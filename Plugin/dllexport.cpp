#include "dllexport.h"

// -------------------------------------------------------------------------- //
// Specialized API Exports
// -------------------------------------------------------------------------- //

extern "C" int UNITY_INTERFACE_EXPORT UNITY_INTERFACE_API PluginFunc()
{
	return Plugin.PluginFunc();
}

extern "C" TextureData UNITY_INTERFACE_EXPORT UNITY_INTERFACE_API GetTexture()
{
	return Plugin.GetTexture();
}
