#include "dllexport.h"

OutputPlugin Plugin;

// -------------------------------------------------------------------------- //
// UnityPlugin Generic Export
// -------------------------------------------------------------------------- //

extern "C" bool UNITY_INTERFACE_EXPORT UNITY_INTERFACE_API EnablePlugin()
{
	return Plugin.OnEnable();
}

extern "C" void UNITY_INTERFACE_EXPORT UNITY_INTERFACE_API DisablePlugin()
{
	Plugin.OnDisable();
}

static void UNITY_INTERFACE_API OnGraphicsDeviceEvent(UnityGfxDeviceEventType eventType)
{
	Plugin.OnGraphicsDeviceEvent(eventType);
}

extern "C" void	UNITY_INTERFACE_EXPORT UNITY_INTERFACE_API UnityPluginLoad(IUnityInterfaces* unityInterfaces)
{
	Plugin.Load(unityInterfaces);
	Plugin.getUnityGraphics()->RegisterDeviceEventCallback(OnGraphicsDeviceEvent);
}

extern "C" void UNITY_INTERFACE_EXPORT UNITY_INTERFACE_API UnityPluginUnload()
{
	Plugin.getUnityGraphics()->UnregisterDeviceEventCallback(OnGraphicsDeviceEvent);
	Plugin.Unload();
}

extern "C" void UNITY_INTERFACE_EXPORT UNITY_INTERFACE_API SetTexture(int texId, void* texPtr, int w, int h)
{
	Plugin.SetTexture(texId, texPtr, w, h);
}

extern "C" TexturePacket UNITY_INTERFACE_EXPORT UNITY_INTERFACE_API GetTexture(int texId)
{
	return Plugin.GetTexture(texId);
}

static void UNITY_INTERFACE_API OnRenderEvent(int eventId)
{
	Plugin.OnRender(eventId);
}

extern "C" UnityRenderingEvent UNITY_INTERFACE_EXPORT UNITY_INTERFACE_API GetRenderEventFunc()
{
	return OnRenderEvent;
}