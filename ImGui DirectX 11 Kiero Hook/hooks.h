#pragma once
#include "includes.h"
#include "iostream"

/*insert your hooks here
you can do this by dumping the game with Il2CPPDumper then
converting the address decimal to hex (https://www.rapidtables.com/convert/number/decimal-to-hex.html)
then putting it in the offsets file and copying the same parameters, basically replicating the function
you must create a bool or a void or a const char depending on what are you trying to hook,
in every function there will be a DWORD as the first param and another DWORD as the last one
between the dwords you replicate your function's parameters
you can also add a function in functions.h to hook this*/

bool(__fastcall* OwnsCosmetic_o)(DWORD*, const char*, const char*, DWORD*);
bool __fastcall OwnsCosmetic_h(DWORD* __this, const char* cosmeticType, const char* id, DWORD* method)
{
    return true;
}
