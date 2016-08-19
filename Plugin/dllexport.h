#pragma once

#include "PluginTemplate.h"

typedef PluginTemplate OutputPlugin;

extern OutputPlugin Plugin;

inline OutputPlugin & GetUnityPlugin() { return Plugin; }