#pragma once
#include "includes.h";
#include "game.h";
#include "offsets.h";
#include "hooks.h";
#include "services.h";

void unlockAll() {
	Hook(OwnsCosmetic, reinterpret_cast<LPVOID>(OwnsCosmetic_h), reinterpret_cast<LPVOID*>(&OwnsCosmetic_o));
}
