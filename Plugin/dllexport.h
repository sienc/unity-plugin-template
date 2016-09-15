#pragma once

#ifdef _USRDLL

#include "PluginTemplate.h"

typedef PluginTemplate OutputPlugin;

extern OutputPlugin Plugin;

inline OutputPlugin & GetUnityPlugin() { return Plugin; }

#endif _USRDLL