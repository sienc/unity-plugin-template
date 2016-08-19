using UnityEngine;
using System;
using System.Collections;
using System.Runtime.InteropServices;

public class PluginTemplate : NativePlugin {

    public int pluginFuncVal = 0;

    private const int TexId = 0;

    Texture2D tex;

    [DllImport(DllName)]
    private static extern int PluginFunc();

    [DllImport(DllName)]
    private static extern TextureData GetTexture();

    // Use this for initialization
    void Start () {
        pluginFuncVal = PluginFunc();

        tex = CreateExternalTexture(TexId);

        // debug
        var renderer = GetComponent<Renderer>();

        if (renderer)
            renderer.material.mainTexture = tex;
    }

    // Update is called once per frame
    void Update () {

    }
}
