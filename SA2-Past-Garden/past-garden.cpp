#include "pch.h"
#include "Objects.h"
#include "StartPos.h"
#include "UsercallFunctionHandler.h"
#include "FastFunctionHook.hpp"

static LandTableInfo* PastLandInfo = nullptr;
static NJS_TEXNAME PAST01_TEXNAME[83]{};
NJS_TEXLIST PAST01_TEXLIST = { arrayptrandlength(PAST01_TEXNAME, Uint32) };
static const TexPackInfo PAST01_TEXINFO = { "PAST01_DC", &PAST01_TEXLIST };
static bool cameOnce = false;
float OOBLimit = -150.0f;

FastFunctionHook<void, ObjectMaster*> ChaoGardenNeutral_Delete_h(0x54CC10);

void Init_NewTreePos() {
	TreePos[0] = { 269, -1, 444 };
	TreePos[1] = { -351, 0, -227 };
	TreePos[2] = { 327, 0, 247 };
}

void Delete_PastGarden(ObjectMaster* obj) {

	ChaoGardenNeutral_Delete_h.Original(obj);

	PrintDebug("Delete Past Garden!\n");

	if (PastLandInfo) 
	{
		delete(PastLandInfo);
		PastLandInfo = nullptr;
	}

	TikalChao_Delete();
	FreeObjectsGarden();

	njReleaseTexture(&PAST01_TEXLIST);
	njReleaseTexture((NJS_TEXLIST*)0xB09818);

	if (WaterRippleFunc_ptr)
	{
		WaterRippleFunc_ptr = 0;
	}

	if (ExecFunc_ptr)
	{
		ExecFunc_ptr = 0;
	}

}

void Chao_OOBLimit_r()
{
	Float* posY;

	if (CurrentChaoArea == PastGarden)
	{
		posY = &MainCharObj1[0]->Position.y;
		if (MainCharObj1[0]->Position.y > 310.0)
		{
			*posY = 310.0;
		}
		if (OOBLimit > *posY)
		{
			*posY = -30.0f;
		}

		return;
	}

	if (CurrentChaoArea > 0 && CurrentChaoArea <= 3)
	{
		posY = &MainCharObj1[0]->Position.y;
		if (MainCharObj1[0]->Position.y > 110.0f)
		{
			*posY = 110.0f;
		}
		if (flt_1657CEC > (double)*posY)
		{
			*posY = 0.5f;
		}
	}
}

void Load_PastGarden()
{
	LandTableSA2BModels = 0;
	LoadLandTable("resource\\gd_PC\\past-garden.sa2lvl", &PastLandInfo, &PAST01_TEXINFO);
	Load_skyboxModel();

	if (!WaterRippleFunc_ptr)
	{
		WaterRippleFunc_ptr = ObjectWaterripple_Load;
	}


	if (!ExecFunc_ptr)
	{
		ExecFunc_ptr = Exec_Load;
	}


	LoadSplashTextures();
}

void Manage_SoundEffectWater(ObjectMaster* a1, NJS_VECTOR pos, char vol)
{
	EntityData1* data;
	unsigned __int16 timer;
	int v3;

	data = a1->Data1.Entity;

	if (CurrentChaoArea == NextChaoArea)
	{
		Play3DSound_EntityAndPos((EntityData1*)a1, 1, &pos, vol);
	}
	timer = data->Timer;
	data->Timer = timer + 1;
	if (!(timer % 900) && njRandom() < 0.4000000059604645f)
	{
		pos.x = njRandom() - 0.5f * 150.0f + 25.0f;
		pos.y = 0.0f;
		pos.z = njRandom() - 0.5f * 150.0f - 75.0f;

		if (CurrentChaoArea == NextChaoArea)
		{
			v3 = (int)(njRandom() * 300.0f + 120.0f);
			Play3DSound_EntityAndPos((EntityData1*)a1, 1, &pos, vol);
		}
	}
}

void SpawnAllElements()
{
	AL_MinimalCreate();
	Load_ChaoTree();
	NJS_VECTOR pos = { 141, -0, 349 };
	NJS_VECTOR vel = { 0, 0, 0 };

	unsigned int save = ChaoSaveIndexThing;
	if (ChaoSaveIndexThing != 1)
	{
		save = 0;
	}

	if ((ChaoToysUnlocked[12955 * save] & 8) != 0)
	{
		ALO_Ball_Load(&pos, &vel);
	}
}

void CameraChildObj(ObjectMaster* a1)
{
	EntityData1* data = a1->Data1.Entity;

	if (!PointerToNormalCamera || !MainCharObj1[0] || CurrentLevel != LevelIDs_ChaoWorld || CurrentChaoArea != PastGarden)
		return;

	switch (data->Action)
	{
	case 0:
		data->Position = { -1, 0, 1365 };
		data->Action++;	
		break;
	case 1:
		if (IsPlayerInsideSphere(&data->Position, 60))
		{
			data->Action++;
		}
		PointerToNormalCamera->Data1.Entity->Position.y = MainCharObj1[0]->Position.y + 10;
		PointerToNormalCamera->Data1.Entity->Position.z = MainCharObj1[0]->Position.z + 30;
		break;
	case 2:

		if (MainCharObj1[0]->Position.z > 1400) {
			PointerToNormalCamera->Data1.Entity->Position.y = MainCharObj1[0]->Position.y + 15;
		}
		else if (MainCharObj1[0]->Position.z < 1250 && MainCharObj1[0]->Position.z > 600) {
			if (MainCharObj1[0]->Rotation.y > 0x8000) {
				PointerToNormalCamera->Data1.Entity->Position.y = MainCharObj1[0]->Position.y + 20;
				PointerToNormalCamera->Data1.Entity->Position.z = MainCharObj1[0]->Position.z + 60;
			}
		}
		break;
	}
}

void __cdecl Past_Garden_Manager(ObjectMaster* a1)
{
	EntityData1* data = a1->Data1.Entity;
	int rot = -0x4000;
	EntityData1* P1 = MainCharObj1[0];

	if (!P1 && data->Action > 0)
		return;

	NJS_VECTOR posWater = { 219.0f, -30.00f, 1077.0f };
	NJS_VECTOR posFountain = { 10, 85, 31 };


	switch (data->Action)
	{
	case 0:

		if (++data->Timer == 5) {
			Move_WayPoints_ToNewPose();
			a1->DisplaySub_Delayed1 = PastGarden_DelayedDisplay;
			data->Action++;
		}

		break;
	case 1:
		if (LastChaoArea != 7) { //chao lobby
			P1->Position = startPosAltar;
			P1->Rotation.y = rot;
			SpawnAllElements();
			data->Action = 3;
			return;
		}
		else
		{
			P1->Position = cameOnce ? newStartPos : startPos;
			P1->Rotation.y = rot;
			data->Position = { 9, 7, 475 };
			LoadChildObject(LoadObj_Data1, CameraChildObj, a1);
		}

		if (MainCharObj1[1]) {
			MainCharObj1[1]->Position = MainCharObj1[0]->Position;
			MainCharObj1[1]->Position.x += 7;
			MainCharObj1[1]->Rotation.y = rot;
		}

		data->Action++;

		break;
	case 2:

		if (IsPlayerInsideSphere(&data->Position, 100))
		{
			cameOnce = true;
			UpdateStartPos();
			SpawnAllElements();
			data->Action++;
		}
		break;
	case 3:

		if ((MainCharObj2[0]->CharID2 == Characters_Tikal) || MainCharObj2[1] && MainCharObj2[1]->CharID2 == Characters_Tikal) {
			data->Action++;
			break;
		}

		LoadObject(2, "Tikal", Tikal_Event, LoadObj_Data1);
		data->Action++;
		break;
	}

	if (data->Action >= 2)
	{

		if (P1->Position.z < 450)
			Manage_SoundEffectWater(a1, posFountain, 6);
		else
			Manage_SoundEffectWater(a1, posWater, 10);

		Animate_Water();
	}
}


void init_PastGarden_Level()
{
	WriteCall((void*)0x54C80F, Load_PastGarden);

	WriteJump((void*)0x54C690, PastGarden_Display); //prevent display to run
	WriteJump(Chao_OOBLimit, Chao_OOBLimit_r); //fix OOB limit

	Init_NewStartPos();
	Init_NewTreePos();
	init_LoadSetObj();
	init_Music();

	WriteJump((void*)0x54C550, Past_Garden_Manager);

	init_ChaoFixes_Hack();

	//issues is below
	// 
	// 
	//prevent the game to load a ton of chao stuff we will call them after.
	//WriteData<5>((int*)0x54C9C2, 0x90);  //spawn chao create	
	WriteData<5>((int*)0x54C9B8, 0x90);	//MinimalCreate
	WriteData<5>((int*)0x54C9C7, 0x90);  //Chao Tree
	WriteData<5>((int*)0x54CA95, 0x90); //ball toy (col despawn if too far)

	init_WaterHack();

	ChaoGardenNeutral_Delete_h.Hook(Delete_PastGarden);

	initTimeOfDay_Hack();
	initNewWayPoints();
}