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
}

public class PluginTemplate : NativePlugin {

    public int pluginFuncVal = 0;

    TextureData texData;
    Texture2D tex;

    [DllImport(DllName)]
    private static extern int PluginFunc();

    [DllImport(DllName)]
    private static extern TextureData GetTexture();

    // Use this for initialization
    void Start () {
        pluginFuncVal = PluginFunc();

        texData = GetTexture();
        tex = Texture2D.CreateExternalTexture(texData.width, texData.height, TextureFormat.RGBA32, true, true, texData.texPtr);

        //tex = CreateTexture(0, 640, 480);

        // debug
        var renderer = GetComponent<Renderer>();

        if (renderer)
            renderer.material.mainTexture = tex;
    }

    // Update is called once per frame
    void Update () {

    }
}
