#ifdef _USRDLL

#include "UnityTexture.h"
#include "dllexport.h"

UnityTexture::UnityTexture()
{
}

UnityTexture::UnityTexture(void * texPtr, int width, int height)
{
	set(texPtr, width, height);
}

UnityTexture::~UnityTexture()
{
}

bool UnityTexture::create(int width, int height)
{
	// release current resources
	release();

	switch (Plugin.deviceType())
	{

	#if SUPPORT_D3D11
	case kUnityGfxRendererD3D11:
	{
		D3D11_TEXTURE2D_DESC desc;
		desc.Width = width;
		desc.Height = height;
		desc.MipLevels = desc.ArraySize = 1;
		desc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
		desc.SampleDesc.Count = 1;
		desc.SampleDesc.Quality = 0;
		desc.Usage = D3D11_USAGE_DEFAULT;
		desc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
		desc.CPUAccessFlags = 0;
		desc.MiscFlags = 0;

		ID3D11Texture2D *texPtr = nullptr;
		HRESULT res = Plugin.getD3D11Device()->CreateTexture2D(&desc, NULL, &texPtr);

		if (res != S_OK || !texPtr)
			break;

		ID3D11ShaderResourceView *resView = nullptr;
		res = Plugin.getD3D11Device()->CreateShaderResourceView(texPtr, NULL, &resView);

		if (res != S_OK || !resView)
			break;

		mTexturePointer = texPtr;
		mExternalPointer = resView;
		mWidth = width;
		mHeight = height;
		mIsOwned = true;

		return true;
	}
	#endif

	#if SUPPORT_OPENGL_LEGACY
	case kUnityGfxRendererOpenGL:
	{
		GLuint texId = 0;
		GLint internalformat = GL_RGBA;
		GLenum format = GL_RGBA;
		GLenum type = GL_UNSIGNED_BYTE;
		const void *pixels = NULL;
		
		// setup texture
		glGenTextures(1, &texId);

		if (texId == 0)
			break;

		mTexturePointer = UINT2PTR(texId);
		mWidth = width;
		mHeight = height;
		mIsOwned = true;

		glActiveTexture(0);
		glBindTexture(GL_TEXTURE_2D, texId);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAX_LEVEL, 0);

		glTexImage2D(GL_TEXTURE_2D, 0, internalformat, width, height, 0, format, type, pixels);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);

		GLfloat fLargest;
		glGetFloatv(GL_MAX_TEXTURE_MAX_ANISOTROPY_EXT, &fLargest);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAX_ANISOTROPY_EXT, fLargest);

		glBindTexture(GL_TEXTURE_2D, 0);

		return true;
	}
	#endif

	}

	return false;
}

void UnityTexture::release()
{
	if (!isValid())
		return;

	if (mIsOwned)
	{
		switch (Plugin.deviceType())
		{

		#if SUPPORT_D3D11
		case kUnityGfxRendererD3D11:
		{
			static_cast<ID3D11Texture2D *>(mTexturePointer)->Release();
			break;
		}
		#endif

		#if SUPPORT_OPENGL_LEGACY
		case kUnityGfxRendererOpenGL:
		{
			GLuint texId = PTR2UINT(mTexturePointer);
			glDeleteTextures(1, &texId);
			break;
		}
		#endif

		}
	}

	mTexturePointer = nullptr;
	mWidth = 0;
	mHeight = 0;
	mIsOwned = false;
}

void UnityTexture::set(void * texPtr, int width, int height)
{
	release();

	mTexturePointer = texPtr;
	mWidth = width;
	mHeight = height;
	mIsOwned = false;
}

void UnityTexture::write(const uint8_t * buffer, int width, int height, int stride)
{
	if (!buffer)
		return;

#if SUPPORT_D3D9
	// D3D9 case
	if (Plugin.deviceType() == kUnityGfxRendererD3D9)
	{
		// Update native texture from code
		if (mTexturePointer)
		{
			IDirect3DTexture9* d3dtex = (IDirect3DTexture9*)mTexturePointer;
			D3DSURFACE_DESC desc;
			d3dtex->GetLevelDesc(0, &desc);
			D3DLOCKED_RECT lr;
			d3dtex->LockRect(0, &lr, NULL, 0);
			FillBuffer((unsigned char*)lr.pBits, buffer, desc.Width, desc.Height, lr.Pitch);
			d3dtex->UnlockRect(0);
		}
	}
#endif


#if SUPPORT_D3D11
	// D3D11 case
	if (Plugin.deviceType() == kUnityGfxRendererD3D11)
	{
		ID3D11DeviceContext* ctx = NULL;
		Plugin.getD3D11Device()->GetImmediateContext(&ctx);

		// update native texture from code
		if (mTexturePointer)
		{
			ID3D11Texture2D* d3dtex = (ID3D11Texture2D*)mTexturePointer;
			D3D11_TEXTURE2D_DESC desc;
			d3dtex->GetDesc(&desc);

			ctx->UpdateSubresource(d3dtex, 0, NULL, buffer, width * stride, 0);

		}

		ctx->Release();
	}
#endif

#if SUPPORT_D3D12
	// D3D12 case
	if (s_DeviceType == kUnityGfxRendererD3D12)
	{
		ID3D12Device* device = s_D3D12->GetDevice();
		ID3D12CommandQueue* queue = s_D3D12->GetCommandQueue();

		// Wait on the previous job (example only - simplifies resource management)
		if (s_D3D12Fence->GetCompletedValue() < s_D3D12FenceValue)
		{
			s_D3D12Fence->SetEventOnCompletion(s_D3D12FenceValue, s_D3D12Event);
			WaitForSingleObject(s_D3D12Event, INFINITE);
		}

		// Update native texture from code
		if (mTexturePointer)
		{
			ID3D12Resource* resource = (ID3D12Resource*)mTexturePointer;
			D3D12_RESOURCE_DESC desc = resource->GetDesc();

			// Begin a command list
			s_D3D12CmdAlloc->Reset();
			s_D3D12CmdList->Reset(s_D3D12CmdAlloc, nullptr);

			// Fill data
			const UINT64 kDataSize = desc.Width*desc.Height * 4;
			ID3D12Resource* upload = GetD3D12UploadResource(kDataSize);
			void* mapped = NULL;
			upload->Map(0, NULL, &mapped);
			FillTextureFromCode(desc.Width, desc.Height, desc.Width * 4, (unsigned char*)mapped);
			upload->Unmap(0, NULL);

			D3D12_TEXTURE_COPY_LOCATION srcLoc = {};
			srcLoc.pResource = upload;
			srcLoc.Type = D3D12_TEXTURE_COPY_TYPE_PLACED_FOOTPRINT;
			device->GetCopyableFootprints(&desc, 0, 1, 0, &srcLoc.PlacedFootprint, nullptr, nullptr, nullptr);

			D3D12_TEXTURE_COPY_LOCATION dstLoc = {};
			dstLoc.pResource = resource;
			dstLoc.Type = D3D12_TEXTURE_COPY_TYPE_SUBRESOURCE_INDEX;
			dstLoc.SubresourceIndex = 0;

			// Queue data upload
			const D3D12_RESOURCE_STATES kDesiredState = D3D12_RESOURCE_STATE_COPY_DEST;
			D3D12_RESOURCE_STATES resourceState = kDesiredState;
			s_D3D12->GetResourceState(resource, &resourceState); // Get resource state as it will be after all command lists Unity queued before this plugin call execute.
			if (resourceState != kDesiredState)
			{
				D3D12_RESOURCE_BARRIER barrierDesc = {};
				barrierDesc.Type = D3D12_RESOURCE_BARRIER_TYPE_TRANSITION;
				barrierDesc.Transition.pResource = resource;
				barrierDesc.Transition.Subresource = D3D12_RESOURCE_BARRIER_ALL_SUBRESOURCES;
				barrierDesc.Transition.StateBefore = resourceState;
				barrierDesc.Transition.StateAfter = kDesiredState;
				s_D3D12CmdList->ResourceBarrier(1, &barrierDesc);
				s_D3D12->SetResourceState(resource, kDesiredState); // Set resource state as it will be after this command list is executed.
			}

			s_D3D12CmdList->CopyTextureRegion(&dstLoc, 0, 0, 0, &srcLoc, nullptr);

			// Execute the command list
			s_D3D12CmdList->Close();
			ID3D12CommandList* lists[1] = { s_D3D12CmdList };
			queue->ExecuteCommandLists(1, lists);
		}

		// Insert fence
		++s_D3D12FenceValue;
		queue->Signal(s_D3D12Fence, s_D3D12FenceValue);
	}
#endif

#if SUPPORT_OPENGL_LEGACY
	// OpenGL 2 legacy case (deprecated)
	if (Plugin.deviceType() == kUnityGfxRendererOpenGL)
	{
		// update native texture from code
		if (mTexturePointer)
		{
			GLuint gltex = (GLuint)(size_t)(mTexturePointer);
			glBindTexture(GL_TEXTURE_2D, gltex);
			//int texWidth, texHeight;
			//glGetTexLevelParameteriv (GL_TEXTURE_2D, 0, GL_TEXTURE_WIDTH, &texWidth);
			//glGetTexLevelParameteriv (GL_TEXTURE_2D, 0, GL_TEXTURE_HEIGHT, &texHeight);

			//unsigned char* data = new unsigned char[texWidth*texHeight*4];
			//FillTextureFromCode (texWidth, texHeight, texWidth*4, data);
			//glTexSubImage2D (GL_TEXTURE_2D, 0, 0, 0, texWidth, texHeight, GL_RGBA, GL_UNSIGNED_BYTE, data);
			//delete[] data;

			glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, width, height, GL_RGBA, GL_UNSIGNED_BYTE, buffer);
		}
	}
#endif

#if SUPPORT_OPENGL_UNIFIED
#define BUFFER_OFFSET(i) ((char *)NULL + (i))

	// OpenGL ES / core case
	if (s_DeviceType == kUnityGfxRendererOpenGLES20 ||
		s_DeviceType == kUnityGfxRendererOpenGLES30 ||
		s_DeviceType == kUnityGfxRendererOpenGLCore)
	{
		assert(glGetError() == GL_NO_ERROR); // Make sure no OpenGL error happen before starting rendering

											 // update native texture from code
		if (mTexturePointer)
		{
			GLuint gltex = (GLuint)(size_t)(mTexturePointer);
			glBindTexture(GL_TEXTURE_2D, gltex);
			// The script only pass width and height with OpenGL ES on mobile
#if SUPPORT_OPENGL_CORE
			if (s_DeviceType == kUnityGfxRendererOpenGLCore)
			{
				glGetTexLevelParameteriv(GL_TEXTURE_2D, 0, GL_TEXTURE_WIDTH, &mWidth);
				glGetTexLevelParameteriv(GL_TEXTURE_2D, 0, GL_TEXTURE_HEIGHT, &mHeight);
			}
#endif

			unsigned char* data = new unsigned char[mWidth*mHeight * 4];
			FillTextureFromCode(mWidth, mHeight, mWidth * 4, data);
			glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, mWidth, mHeight, GL_RGBA, GL_UNSIGNED_BYTE, data);
			delete[] data;
		}

		assert(glGetError() == GL_NO_ERROR);
	}
#endif
}

void UnityTexture::FillBuffer(unsigned char * dst, const uint8_t *src, int width, int height, int stride)
{
	if (!src || !dst)
		return;

	const uint8_t *ptrSrc = src;

	for (int y = 0; y < height; ++y)
	{
		uint8_t *ptrDst = dst;
		for (int x = 0; x < width; ++x)
		{
			// Simple oldskool "plasma effect", a bunch of combined sine waves

			// Write the texture pixel
			ptrDst[0] = ptrSrc[0];
			ptrDst[1] = ptrSrc[1];
			ptrDst[2] = ptrSrc[2];
			ptrDst[3] = ptrSrc[3];

			// To next pixel (our pixels are 4 bpp)
			ptrDst += 4;
			ptrSrc += 4;
		}

		// To next image row
		dst += stride;
	}
}

#endif _USRDLL