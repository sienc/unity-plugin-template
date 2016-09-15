#ifdef _USRDLL

#include "PluginTemplate.h"

void PluginTemplate::Load(IUnityInterfaces * unityInterfaces)
{
	UnityPlugin::Load(unityInterfaces);
}

void PluginTemplate::Unload()
{
	UnityPlugin::Unload();
}

bool PluginTemplate::OnEnable()
{
	if (!UnityPlugin::OnEnable())
		return false;

	mTexture.create(640, 480);

	return true;
}

void PluginTemplate::OnDisable()
{
	mTexture.release();
	UnityPlugin::OnDisable();
}

void PluginTemplate::OnRender(int eventId)
{
	UnityPlugin::OnRender(eventId);
}

TexturePacket PluginTemplate::GetTexture(int texId)
{
	return mTexture.toPacket();
}

#endif // _USRDLL
