#include "dllexport.h"

// -------------------------------------------------------------------------- //
// Specialized API Exports
// -------------------------------------------------------------------------- //

extern "C" int UNITY_INTERFACE_EXPORT UNITY_INTERFACE_API PluginFunc()
{
	return Plugin.PluginFunc();
}

extern "C" DataStruct UNITY_INTERFACE_EXPORT UNITY_INTERFACE_API ReturnStruct()
{
	return Plugin.ReturnStruct();
}

extern "C" int UNITY_INTERFACE_EXPORT UNITY_INTERFACE_API SetStruct(DataStruct *data)
{
	data->intVal1 = 4;
	data->intVal2 = 5;
	data->intVal3 = 6;

	return -1;

	//return Plugin.SetStruct(data);
}