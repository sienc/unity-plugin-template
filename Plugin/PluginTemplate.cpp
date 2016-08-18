#include "PluginTemplate.h"

#ifdef _USRDLL

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
	return UnityPlugin::OnEnable();
}

void PluginTemplate::OnDisable()
{
	UnityPlugin::OnDisable();
}

void PluginTemplate::OnRender(int eventId)
{
	UnityPlugin::OnRender(eventId);
}

#endif // _USRDLL
