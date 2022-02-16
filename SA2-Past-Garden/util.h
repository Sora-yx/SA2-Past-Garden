#pragma once

void LoadLandTable(const char* path, LandTableInfo** land, const TexPackInfo* tex);
ModelInfo* LoadMDL(const char* name, ModelFormat format);
void __cdecl ObjectGeneric_DisplayV(ObjectMaster* obj);
int IsPlayerInsideSphere(NJS_VECTOR* position, float a2);
void ResetPlayerSpeed(char pid);
void FreeMDL(ModelInfo* pointer);
AnimationFile* LoadAnim(const char* name);
void FreeAnim(AnimationFile* pointer);
void DoNextAction_r(int playerNum, char action, int unknown);

void LookAt(NJS_VECTOR* from, NJS_VECTOR* to, Angle* outx, Angle* outy);
void PlayerLookAt(NJS_VECTOR* from, NJS_VECTOR* to, Angle* outx, Angle* outy);