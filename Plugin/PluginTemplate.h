#pragma once

#ifdef _USRDLL

#include "UnityPlugin.h"

extern "C"
{
	typedef struct _DataStruct
	{
		int intVal1;
		int intVal2;
		int intVal3;
	} DataStruct;
}

class PluginTemplate : public UnityPlugin
{
public:
	// UnityPlugin Overrides
	virtual void Load(IUnityInterfaces* unityInterfaces) override;
	virtual void Unload() override;

	virtual bool OnEnable() override;
	virtual void OnDisable() override;

	virtual void OnRender(int eventId) override;

	// Specialized API exports
	int PluginFunc() { return 123; }
	DataStruct ReturnStruct() {
		DataStruct data;
		data.intVal1 = 1;
		data.intVal2 = 2;
		data.intVal3 = 3;
		return data;
	}

	int SetStruct(DataStruct *data) {
		data->intVal1 = 1;
		data->intVal2 = 2;
		data->intVal3 = 3;

		return -1;
	}
};

#endif // _USRDLL
