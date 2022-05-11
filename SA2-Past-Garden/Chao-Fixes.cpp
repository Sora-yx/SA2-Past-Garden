#include "pch.h"

Trampoline* sub_54AC70_t = nullptr;
Trampoline* ALO_Ball_Main2_t = nullptr;


//a Big garden creates stupid bugs if the player is too far, we hack some functions to prevent them to run if so.

static int __cdecl sub_54AC70_orig(ObjectMaster* obj, float a2, float a3, float a4)
{
	auto target = sub_54AC70_t->Target();

	int result;
	__asm
	{
		push[a4]
		push[a3]
		push[a2]
		mov eax, [obj]
		call target
		add esp, 12
		mov result, eax
	}
	return result;
}

int sub_54AC70_r(ObjectMaster* obj, float a2, float a3, float a4)
{
	if (isInPastGarden())
	{
		if (MainCharObj1[0])
		{
			if (!IsPlayerInsideSphere(&obj->Data1.Entity->Position, 800))
				return 0;
		}
	}

	return sub_54AC70_orig(obj, a2, a3, a4);
}

static void __declspec(naked) sub_54AC70ASM()
{
	__asm
	{
		push[esp + 0Ch] // a4
		push[esp + 0Ch] // a3
		push[esp + 0Ch] // a2
		push eax // eax0
		call sub_54AC70_r
		add esp, 4
		add esp, 4
		add esp, 4
		add esp, 4
		retn
	}
}


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
	if (isInPastGarden())
		landColRadius = 10000.0f;
}


void init_ChaoFixes_Hack()
{
	//sub_54AC70_t = new Trampoline((int)0x54AC70, (int)0x54AC76, sub_54AC70ASM);
	ALO_Ball_Main2_t = new Trampoline((int)ALO_Ball_Main2, (int)ALO_Ball_Main2 + 0x6, ALO_Ball_Main2_r); //fix nonsense race entry crash shit

	WriteCall((void*)0x43CF86, CWE_PosYFixes); //fix pos Y issue with Chao World Extended mod


	return;
}