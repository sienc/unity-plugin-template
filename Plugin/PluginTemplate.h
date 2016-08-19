#pragma once

#ifdef _USRDLL

#include "UnityPlugin.h"
#include "UnityTexture.h"

class PluginTemplate : public UnityPlugin
{
private:
	UnityTexture mTexture;

public:
	// UnityPlugin Overrides
	virtual void Load(IUnityInterfaces* unityInterfaces) override;
	virtual void Unload() override;

	virtual bool OnEnable() override;
	virtual void OnDisable() override;

	virtual void OnRender(int eventId) override;

	//virtual void SetTexture(int texId, void* texPtr, int width, int height) override;
	virtual TexturePacket GetTexture(int texId) override;

	// Specialized API exports
	int PluginFunc() { return 123; }
	
};

#endif // _USRDLL
