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
	if (!UnityPlugin::OnEnable())
		return false;

	mTexture.create(640, 480);

	//uint8_t *data = new uint8_t[640 * 480 * 4];
	//uint8_t *dst = data;

	//for (int y = 0; y < 480; y++)
	//{
	//	for (int x = 0; x < 640; x++)
	//	{
	//		dst[0] = 0;
	//		dst[1] = 0xff;
	//		dst[2] = 0;
	//		dst[3] = 0xff;
	//		dst += 4;
	//	}
	//}

	//yup::FrameBuffer buffer(640, 480);
	//buffer.setData(data);
	//mTexture.write(&buffer);

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

	if (mTexture.isValid())
	{
		uint8_t *data = new uint8_t[640 * 480 * 4];
		uint8_t *dst = data;

		for (int y = 0; y < 480; y++)
		{
			for (int x = 0; x < 640; x++)
			{
				dst[0] = 0;
				dst[1] = 0xff;
				dst[2] = 0;
				dst[3] = 0xff;
				dst += 4;
			}
		}

		yup::FrameBuffer buffer(640, 480);
		buffer.setData(data);
		mTexture.write(&buffer);

		delete[] data;
	}
}

void PluginTemplate::SetTexture(int texId, void* texPtr, int width, int height)
{
	//mTexture.assign(texPtr, width, height);
}

#endif // _USRDLL
