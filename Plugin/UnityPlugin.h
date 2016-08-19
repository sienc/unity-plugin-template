#pragma once

#ifdef _USRDLL

class UnityPlugin;

#include <vector>

#include "Unity/IUnityInterface.h"
#include "Unity/IUnityGraphics.h"

#include "UnityTexture.h"

// Which platform we are on?
#if _MSC_VER
#define UNITY_WIN 1
#elif defined(__APPLE__)
#if defined(__arm__)
#define UNITY_IPHONE 1
#else
#define UNITY_OSX 1
#endif
#elif defined(UNITY_METRO) || defined(UNITY_ANDROID) || defined(UNITY_LINUX)
// these are defined externally
#else
#error "Unknown platform!"
#endif

// Which graphics device APIs we possibly support?
#if UNITY_METRO
#define SUPPORT_D3D11 1
#if WINDOWS_UWP
#define SUPPORT_D3D12 1
#endif
#elif UNITY_WIN
#define SUPPORT_D3D9 1
#define SUPPORT_D3D11 1 // comment this out if you don't have D3D11 header/library files
#ifdef _MSC_VER
#if _MSC_VER >= 1900
//			#define SUPPORT_D3D12 1
#endif
#endif
#define SUPPORT_OPENGL_LEGACY 1
#define SUPPORT_OPENGL_UNIFIED 0
#define SUPPORT_OPENGL_CORE 1
#elif UNITY_IPHONE || UNITY_ANDROID
#define SUPPORT_OPENGL_UNIFIED 1
#define SUPPORT_OPENGL_ES 1
#elif UNITY_OSX || UNITY_LINUX
#define SUPPORT_OPENGL_LEGACY 1
#define SUPPORT_OPENGL_UNIFIED 1
#define SUPPORT_OPENGL_CORE 1
#endif

// --------------------------------------------------------------------------
// Include headers for the graphics APIs we support

#if SUPPORT_D3D9
#	include <d3d9.h>
#	include "Unity/IUnityGraphicsD3D9.h"
#endif
#if SUPPORT_D3D11
#	include <d3d11.h>
#	include "Unity/IUnityGraphicsD3D11.h"
#endif
#if SUPPORT_D3D12
#	include <d3d12.h>
#	include "Unity/IUnityGraphicsD3D12.h"
#endif

#if SUPPORT_OPENGL_LEGACY
#	include "GL/glew.h"
#endif
#if SUPPORT_OPENGL_UNIFIED
#	if UNITY_IPHONE
#		include <OpenGLES/ES2/gl.h>
#	elif UNITY_ANDROID
#		include <GLES2/gl2.h>
#	else
#		include "GL/glew.h"
#	endif
#endif

typedef std::vector<UnityTexture> UnityTextureList;

class UnityPlugin
{
private:
	IUnityInterfaces * mUnityInterfaces = nullptr;
	IUnityGraphics * mGraphics = nullptr;
	UnityGfxRenderer mDeviceType = kUnityGfxRendererNull;

	#if SUPPORT_D3D11
	ID3D11Device * mD3D11Device = nullptr;
	#endif

	#if SUPPORT_D3D12
	const UINT kNodeMask = 0;

	IUnityGraphicsD3D12* s_D3D12 = NULL;
	ID3D12Resource* s_D3D12Upload = NULL;

	ID3D12CommandAllocator* s_D3D12CmdAlloc = NULL;
	ID3D12GraphicsCommandList* s_D3D12CmdList = NULL;

	ID3D12Fence* s_D3D12Fence = NULL;
	UINT64 s_D3D12FenceValue = 1;
	HANDLE s_D3D12Event = NULL;
	#endif

private:
	UnityTextureList mUnityTextureList;

public:
	UnityPlugin();
	~UnityPlugin();

	virtual void Load(IUnityInterfaces* unityInterfaces);
	virtual void Unload();

	virtual bool OnEnable() { return true; }
	virtual void OnDisable() {}

	virtual void OnRender(int eventId);

	void OnGraphicsDeviceEvent(UnityGfxDeviceEventType eventType);
	void SetTexture(int texId, void* texPtr, int width, int height);


	#if SUPPORT_D3D11
	void DoEventGraphicsDeviceD3D11(UnityGfxDeviceEventType eventType);
	ID3D11Device * getD3D11Device() const { return mD3D11Device; }
	#endif

	#if SUPPORT_D3D12
	void DoEventGraphicsDeviceD3D12(UnityGfxDeviceEventType eventType);
	#endif

	IUnityGraphics * getUnityGraphics() const { return mGraphics; }
	UnityGfxRenderer deviceType() const { return mDeviceType; }
};

#endif // _USRDLL
