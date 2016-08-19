using UnityEngine;
using System.Collections;
using System.Runtime.InteropServices;

[StructLayout(LayoutKind.Sequential)]
public struct DataStruct {
    public int intVal1;
    public int intVal2;
    public int intVal3;
}

public class PluginTemplate : NativePlugin {

    public int pluginFuncVal = 0;

    public int data1;
    public int data2;
    public int data3;

    DataStruct data;

    [DllImport(DllName)]
    private static extern int PluginFunc();

    [DllImport(DllName)]
    private static extern DataStruct ReturnStruct();

    [DllImport(DllName)]
    private static extern int SetStruct(ref DataStruct data);

    // Use this for initialization
    void Start () {
        pluginFuncVal = PluginFunc();

        //data1 = SetStruct();

        //data = ReturnStruct();
        //data1 = data.intVal1;
        //data2 = data.intVal2;
        //data3 = data.intVal3;
        data = new DataStruct();

        data.intVal1 = 7;
        data.intVal2 = 8;
        data.intVal3 = 9;

        SetStruct(ref data);

        data1 = data.intVal1;
        data2 = data.intVal2;
        data3 = data.intVal3;

    }

    // Update is called once per frame
    void Update () {

    }
}
