/*@26zylo's IL2CPP SDK
game.h*/

#pragma once
#include "includes.h"

uintptr_t module = (uintptr_t)GetModuleHandle(NULL); // get the module handle
uintptr_t game = (uintptr_t)GetModuleHandle("GameAssembly.dll"); // get the module handle from the il2cpp game
uintptr_t unityplayer = (uintptr_t)GetModuleHandle("UnityPlayer.dll"); // get the module handle from unityplayer