#include "pch.h"

Trampoline* Chao_Main_t = nullptr;
Trampoline* sub_54AC70_t = nullptr;

float ColliDistance = 800.0f;
//a Big garden creates stupid bugs if the player is too far, we hack some functions to prevent them to run if so.

DataPointer(int, ChaoEnabled, 0x01DBE634);

void Chao_Main_r(ObjectMaster* obj)
{
	if (CurrentChaoArea == PastGarden && !IsPlayerInsideSphere(&obj->Data1.Entity->Position, 500))
	{
		return;
	}

	((decltype(Chao_Main_r)*)Chao_Main_t->Target())(obj);
}


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
	if (!IsPlayerInsideSphere(&obj->Data1.Entity->Position, 800))
	{
		return 0;
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

bool isInPastGarden()
{
	return CurrentLevel == LevelIDs_ChaoWorld && CurrentChaoArea == PastGarden;
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

DataPointer(float, FLOAT_00905820, 0x905820);

void init_ChaoFixes_Hack()
{
	//sub_54AC70_t = new Trampoline((int)0x54AC70, (int)0x54AC76, sub_54AC70ASM);
	Chao_Main_t = new Trampoline((int)Chao_Main, (int)Chao_Main + 0x8, Chao_Main_r);

	WriteCall((void*)0x43CF86, CWE_PosYFixes); //fix pos Y issue with Chao World Extended mod

	//WriteData<6>((int*)0x796b5c, 0x90);
	//WriteData<1>((int*)0x796B20, 0xC3);

	WriteData<6>((int*)0x47d639, 0x90);
	WriteData<2>((int*)0x47d43c, 0x90);

	//WriteData((float**)0x47d62B, &ColliDistance);
	return;
}