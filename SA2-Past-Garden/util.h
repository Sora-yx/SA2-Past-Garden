#pragma once

void LoadLandTable(const char* path, LandTableInfo** land, const TexPackInfo* tex);
ModelInfo* LoadMDL(const char* name, ModelFormat format);
void __cdecl ObjectGeneric_DisplayV(ObjectMaster* obj);
int IsPlayerInsideSphere(NJS_VECTOR* position, float a2);
void ResetPlayerSpeed(char pid);