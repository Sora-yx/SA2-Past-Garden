#include "pch.h"

Trampoline* ALO_Ball_Main2_t = nullptr;
Trampoline* InitLandColMemory_t = nullptr;

//a Big garden creates stupid bugs if the player is too far, we hack some functions to prevent them to run if so.

float playerPosY = 0.0f;

void CWE_FixesOnFrames()
{
	if (!isInPastGarden())
		return;

	for (uint8_t i = 0; i < 2; i++) {

		if (!MainCharObj1[i])
			continue;

		MainCharObj1[i]->Position.y = playerPosY;
	}
}

void CWE_PosYFixes() {

	if (isInPastGarden()) {

		for (uint8_t i = 0; i < 2; i++) {

			if (MainCharObj1[i])
			{
				playerPosY = MainCharObj1[i]->Position.y;
			}
		}
	}

	CollisionLoop();
}

void ALO_Ball_Main2_r(ObjectMaster* obj)
{
	if (isInPastGarden())
	{
		if (MainCharObj1[0])
		{
			if (!IsPlayerInsideSphere(&obj->Data1.Entity->Position, 500))
				return;
		}
	}

	((decltype(ALO_Ball_Main2_r)*)ALO_Ball_Main2_t->Target())(obj);
}


//sa2 has a limit of 400 radius before it unload a landtable collision to save resource. 
//this makes objecs and chao clip the floor on big garden, we increase the radius to fix it.
void LandTable_ColRadiusFixes()
{
	if (isInPastGarden()) {
		landColRadius = 10000.0f;
	}

}

void ChangeActiveLandtableColLimit(bool enabled)
{
	return;


	if (enabled)
	{
		//restore vanilla behaviour (128 col max)
		WriteData<1>((int*)0x47D67B, 0x80);
		WriteData<1>((int*)0x47D67C, 0x00);

		WriteData<1>((int*)0x47D484, 0x80);
		WriteData<1>((int*)0x47D485, 0x00);
	}

}

VoidFunc(ResetLandtableVariables, 0x47BCC0);
DataPointer(int, dword_1A5A2D0, 0x1A5A2D0);
DataPointer(int, dword_1A5A2D4, 0x1A5A2D4);
DataPointer(int, MobileColliList, 0x1A5A2E0);
DataPointer(int, LandVisibleEntries, 0x1A5A2E4);
DataPointer(int, dword_1A5A300, 0x1A5A300);
DataPointer(int, dword_19459D0, 0x19459D0);

void InitLandColMemory_r()
{
	if (CurrentLevel != LevelIDs_ChaoWorld || CurrentChaoArea > 3)
	{
		//restore original landtable col limit
		WriteData<1>((int*)0x47D67B, 0x80);
		WriteData<1>((int*)0x47D484, 0x80);

		VoidFunc(origin, InitLandColMemory_t->Target());
		return origin();
	}


	//increase the number of active col check from 128 to 206 (this goes with the 400 landtable col radius limit)
	WriteData<1>((int*)0x47D67B, 0xCE);
	WriteData<1>((int*)0x47D484, 0xCE);

	DWORD* v0; // eax
	DWORD* v1; // eax
	int v2; // eax
	DWORD* v3; // eax
	int v4; // eax
	DWORD* v5; // eax
	DWORD* v6; // eax

	if (!dword_1A5A2D0)
	{
		v0 = (DWORD*)MemoryManager->Allocate(2540, (char*)"..\\..\\src\\land.c", 259);
		*v0 = 0x12345678;
		dword_1A5A2D0 = (int)(v0 + 1);
	}
	if (!dword_1A5A2D4)
	{
		v1 = (DWORD*)MemoryManager->Allocate(5100, (char*)"..\\..\\src\\land.c", 262);
		*v1 = 0x12345678;
		dword_1A5A2D4 = (int)(v1 + 1);
	}
	if (!LandColList)
	{
		v2 = (int)MemoryManager->Allocate(13292, (char*)"..\\..\\src\\land.c", 265);
		*(DWORD*)v2 = 0x12345678;
		LandColList = (DynColInfo*)(v2 + 4);
	}
	if (!MobileColliList)
	{
		v3 = (DWORD*)MemoryManager->Allocate(4076, (char*)"..\\..\\src\\land.c", 268);
		*v3 = 0x12345678;
		MobileColliList = (int)(v3 + 1);
	}
	if (!LandVisibleEntries)
	{
		v4 = (int)MemoryManager->Allocate(5100, (char*)"..\\..\\src\\land.c", 271);
		*(DWORD*)v4 = 0x12345678;
		LandVisibleEntries = (v4 + 4);
	}
	if (!dword_1A5A300)
	{
		v5 = (DWORD*)MemoryManager->Allocate(15340, (char*)"..\\..\\src\\land.c", 274);
		*v5 = 0x12345678;
		dword_1A5A300 = (int)(v5 + 1);
	}
	if (!dword_19459D0)
	{
		v6 = (DWORD*)MemoryManager->Allocate(1260, (char*)"..\\..\\src\\land.c", 277);
		*v6 = 0x12345678;
		dword_19459D0 = (int)(v6 + 1);
	}

	ResetLandtableVariables();
}

void init_ChaoFixes_Hack()
{
	ALO_Ball_Main2_t = new Trampoline((int)ALO_Ball_Main2, (int)ALO_Ball_Main2 + 0x6, ALO_Ball_Main2_r); //fix nonsense race entry crash shit

	WriteCall((void*)0x43CF86, CWE_PosYFixes); //fix pos Y issue with Chao World Extended mod

	//increase the memory assigned to the landtable collision (fix game crash)
	InitLandColMemory_t = new Trampoline((int)0x47BB50, (int)0x47BB57, InitLandColMemory_r);

	return;
}