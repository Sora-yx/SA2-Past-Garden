#include "pch.h"
#include "Objects.h"
#include "StartPos.h"
#include "UsercallFunctionHandler.h"

static LandTableInfo* PastLandInfo = nullptr;
static NJS_TEXNAME PAST01_TEXNAME[83]{};
NJS_TEXLIST PAST01_TEXLIST = { arrayptrandlength(PAST01_TEXNAME, Uint32) };
static const TexPackInfo PAST01_TEXINFO = { "PAST01_DC", &PAST01_TEXLIST };

float OOBLimit = -150.0f;

Trampoline* ChaoGardenNeutral_Delete_t = nullptr;

void Init_NewTreePos() {
	TreePos[0] = { 269, -1, 444 };
	TreePos[1] = { -189, 0, -381 };
	TreePos[2] = { 327, 0, 247 };
}


void Delete_PastGarden(ObjectMaster* obj) {


	((decltype(Delete_PastGarden)*)ChaoGardenNeutral_Delete_t->Target())(obj);


	PrintDebug("Delete Past Garden!\n");

	if (PastLandInfo) {
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

	return;
}
void Chao_OOBLimit_r()
{
	Float* posY; // ecx

	if (CurrentChaoArea == PastGarden)
	{
		posY = &MainCharObj1[0]->Position.y;
		if (MainCharObj1[0]->Position.y > 310.0)
		{
			*posY = 310.0;
		}
		if (OOBLimit > (double)*posY)
		{
			*posY = -30.0f;
		}

		return;
	}

	if (CurrentChaoArea > 0 && CurrentChaoArea <= 3)
	{
		posY = &MainCharObj1[0]->Position.y;
		if (MainCharObj1[0]->Position.y > 110.0)
		{
			*posY = 110.0;
		}
		if (flt_1657CEC > (double)*posY)
		{
			*posY = 0.5;
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

	return;
}

void Manage_SoundEffectWater(ObjectMaster* a1, NJS_VECTOR pos, char vol)
{
	EntityData1* data;
	unsigned __int16 timer;
	int v3;



	data = a1->Data1.Entity;
	//pos.z = -100.0;
	if (CurrentChaoArea == NextChaoArea)
	{
		Play3DSound_EntityAndPos((EntityData1*)a1, 1, &pos, vol);
	}
	timer = data->field_6;
	data->field_6 = timer + 1;
	if (!(timer % 900) && (double)rand() * 0.000030517578125 < 0.4000000059604645)
	{
		pos.x = ((double)rand() * 0.000030517578125 - 0.5) * 150.0 + 25.0;
		pos.y = 0.0;
		pos.z = ((double)rand() * 0.000030517578125 - 0.5) * 150.0 - 75.0;

		if (CurrentChaoArea == NextChaoArea)
		{
			v3 = (int)((double)rand() * 0.000030517578125 * 300.0 + 120.0);
			Play3DSound_EntityAndPos((EntityData1*)a1, 1, &pos, vol);
		}
	}
}


void SpawnAllElements()
{
	AL_MinimalCreate();
	SpawnAllChaoInGarden();
	Load_ChaoTree();
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

		if (++data->field_6 == 5)
			data->Action++;

		break;
	case 1:



		if (P1->Position.z == startPosLeaveCave.z) {
			P1->Rotation.y = rot;
			SpawnAllElements();
			data->Action = 3;
			return;
		}
		else
		{
			P1->Position = startPos;
			P1->Rotation.y = rot;
			data->Position = { 2, -12, 502 };
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

		if (P1->Position.z < 350)
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

	//prevent the game to load a ton of chao stuff we will call them after.
	WriteData<5>((int*)0x54C9B8, 0x90);	//MinimalCreate
	WriteData<5>((int*)0x54C9C2, 0x90);  //spawn chao create	
	WriteData<5>((int*)0x54C9C7, 0x90);  //Chao Tree


	init_ChaoFixes_Hack();
	init_WaterHack();

	ChaoGardenNeutral_Delete_t = new Trampoline((int)0x54CC10, (int)0x54CC15, Delete_PastGarden);

	initTimeOfDay_Hack();
}