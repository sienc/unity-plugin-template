#ifdef _USRDLL

#include "UnityPlugin.h"

UnityPlugin::UnityPlugin()
{
}


UnityPlugin::~UnityPlugin()
{
}

void UnityPlugin::OnGraphicsDeviceEvent(UnityGfxDeviceEventType eventType)
{
	UnityGfxRenderer currentDeviceType = mDeviceType;

	switch (eventType)
	{
	case kUnityGfxDeviceEventInitialize:
	{
		mDeviceType = mGraphics->GetRenderer();
		currentDeviceType = mDeviceType;
		break;
	}

	case kUnityGfxDeviceEventShutdown:
	{
		mDeviceType = kUnityGfxRendererNull;
		//g_TexturePointer = NULL;
		break;
	}

	case kUnityGfxDeviceEventBeforeReset:
	{
		break;
	}

	case kUnityGfxDeviceEventAfterReset:
	{
		break;
	}
	};

#if SUPPORT_D3D11
	if (currentDeviceType == kUnityGfxRendererD3D11)
		DoEventGraphicsDeviceD3D11(eventType);
#endif

#if SUPPORT_D3D12
	if (currentDeviceType == kUnityGfxRendererD3D12)
		DoEventGraphicsDeviceD3D12(eventType);
#endif
}

void UnityPlugin::Load(IUnityInterfaces * unityInterfaces)
{
	mUnityInterfaces = unityInterfaces;
	mGraphics = mUnityInterfaces->Get<IUnityGraphics>();
	OnGraphicsDeviceEvent(kUnityGfxDeviceEventInitialize);
}

void UnityPlugin::Unload()
{
}

void UnityPlugin::SetTexture(int texId, void* texPtr, int width, int height)
{
	//mUnityTextureList.push_back(UnityTexture(texId, texPtr, width, height, *this));
}

TexturePacket UnityPlugin::GetTexture(int texId)
{
	return { NULL, 0, 0 };
}

void UnityPlugin::OnRender(int eventId)
{
	// Unknown graphics device type? Do nothing.
	if (mDeviceType == kUnityGfxRendererNull)
		return;
}

#if SUPPORT_D3D11

void UnityPlugin::DoEventGraphicsDeviceD3D11(UnityGfxDeviceEventType eventType)
{
	if (eventType == kUnityGfxDeviceEventInitialize)
	{
		IUnityGraphicsD3D11* d3d11 = mUnityInterfaces->Get<IUnityGraphicsD3D11>();
		mD3D11Device = d3d11->GetDevice();
	}
}

#endif

#if SUPPORT_D3D12

void UnityPlugin::DoEventGraphicsDeviceD3D12(UnityGfxDeviceEventType eventType)
{
}

#endif

#endif // _USRDLL
