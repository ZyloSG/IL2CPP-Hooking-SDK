#pragma once
#include "includes.h"
#include "game.h"

void Hook(uintptr_t offset, LPVOID hook, LPVOID* original) {
    MH_CreateHook(reinterpret_cast<LPVOID>(game + offset), hook, original);
    MH_EnableHook(reinterpret_cast<LPVOID>(game + offset));
}
