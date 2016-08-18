#pragma once

#ifdef _USRDLL

#include "UnityPlugin.h"

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
};

#endif // _USRDLL
