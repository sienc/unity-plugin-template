// on OpenGL ES there is no way to query texture extents from native texture id
#if (UNITY_IPHONE || UNITY_ANDROID) && !UNITY_EDITOR
#define UNITY_GLES_RENDERER
#endif

using UnityEngine;
using System;
using System.Collections;
using System.Runtime.InteropServices;


[StructLayout(LayoutKind.Sequential)]
public struct TextureData
{
    public IntPtr texPtr;
    public int width;
    public int height;

    public bool isValid()
    {
        return texPtr.ToInt64() != 0;
    }
}


public class NativePlugin : MonoBehaviour {

#if UNITY_IPHONE && !UNITY_EDITOR
    public const string DllName = "__Internal";
#else
    public const string DllName = "PluginTemplate";
#endif

    // ============================== //
    // DLL functions
    // ============================== //

    [DllImport(DllName)]
    private static extern bool EnablePlugin();

    [DllImport(DllName)]
    private static extern void DisablePlugin();

    [DllImport(DllName)]
    private static extern IntPtr GetRenderEventFunc();

    [DllImport(DllName)]
	private static extern void SetTexture(int texId, System.IntPtr texture, int w, int h);

    [DllImport(DllName)]
    private static extern TextureData GetTexture(int texId);

    private IEnumerator CallPluginAtEndOfFrames()
    {
        while (true)
        {
            // Wait until all frame rendering is done
            yield return new WaitForEndOfFrame();
            GL.IssuePluginEvent(GetRenderEventFunc(), 1);
        }
    }

    // Create a texture in Unity and share it with the native plugin
    protected Texture2D CreateSharedTexture(int texId, int width, int height)
    {
        // Create a texture
        Texture2D tex = new Texture2D(width, height, TextureFormat.RGBA32, false);
        tex.filterMode = FilterMode.Point;
        tex.Apply();

		SetTexture(texId, tex.GetNativeTexturePtr(), tex.width, tex.height);

        return tex;
    }

    // Create a Unity texture from a native resource
    protected Texture2D CreateExternalTexture(int texId)
    {
        TextureData texData = GetTexture(texId);
        if (texData.isValid())
            return Texture2D.CreateExternalTexture(texData.width, texData.height, TextureFormat.RGBA32, false, true, texData.texPtr);
        else
            return null;
    }

    void OnEnable()
    {
        if(!EnablePlugin())
        {
            gameObject.SetActive(false);
            return;
        }

        StartCoroutine("CallPluginAtEndOfFrames");
    }

    void OnDisable()
    {
        StopAllCoroutines();
        DisablePlugin();
    }
}
