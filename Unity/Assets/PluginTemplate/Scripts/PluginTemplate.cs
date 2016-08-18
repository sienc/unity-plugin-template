using UnityEngine;
using System.Collections;
using System.Runtime.InteropServices;

public class PluginTemplate : NativePlugin {

    public int pluginFuncVal = 0;

    [DllImport(DllName)]
    private static extern int PluginFunc();

    // Use this for initialization
    void Start () {
        pluginFuncVal = PluginFunc();
    }

    // Update is called once per frame
    void Update () {

    }
}
