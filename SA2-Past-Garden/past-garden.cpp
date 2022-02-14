#include "pch.h"
#include "Objects.h"
#include "StartPos.h"
#include "UsercallFunctionHandler.h"

static LandTableInfo* PastLandInfo = nullptr;
static NJS_TEXNAME PAST01_TEXNAME[83]{};
static NJS_TEXLIST PAST01_TEXLIST = { arrayptrandlength(PAST01_TEXNAME, Uint32) };
static const TexPackInfo PAST01_TEXINFO = { "PAST01_DC", &PAST01_TEXLIST };

float OOBLimit = -150.0f;

void Init_NewTreePos() {
	TreePos[0] = { 269, -1, 444 };
	TreePos[1] = { -189, 0, -381 };
	TreePos[2] = { 327, 0, 247 };
}

void Chao_OOBLimit_r()
{
	Float* posY; // ecx

	if (CurrentChaoArea == 1) //1 is Past Garden aka neutral garden
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
	LoadSplashTextures();
	return;
}

int countTexSea = 73;
int countTexFountain = 59;

void Animate_Water()
{
	if (TimeTotal % 3 == 0) {
		countTexFountain++;
	}

	if (TimeTotal % 4 == 0) {
		countTexSea++;
	}

	if (countTexFountain >= 73)
		countTexFountain = 59;

	if (countTexSea >= 83)
		countTexSea = 73;

	//fountain
	PAST01_TEXLIST.textures[57].texaddr = PAST01_TEXLIST.textures[countTexFountain].texaddr;

	//sea
	PAST01_TEXLIST.textures[6].texaddr = PAST01_TEXLIST.textures[countTexSea].texaddr;
}

void Manage_SoundEffect(ObjectMaster* a1)
{
	EntityData1* data;
	unsigned __int16 timer;
	int v3;
	NJS_VECTOR pos;


	pos = { 219.0f, -30.00f, 1077.0f };
	data = a1->Data1.Entity;
	//pos.z = -100.0;
	if (CurrentChaoArea == NextChaoArea)
	{
		Play3DSound_EntityAndPos((EntityData1*)a1, 1, &pos, 10);
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
			Play3DSound_EntityAndPos((EntityData1*)a1, 1, &pos, 10);
		}
	}

}

DataPointer(ObjectMaster*, dword_1313D40, 0x1313D40);

void SpawnAllElements()
{
	unsigned int v10; // eax
	unsigned int v11; // ecx
	unsigned int v12; // eax
	unsigned int v13; // eax
	unsigned int v14; // eax
	NJS_VECTOR position;
	NJS_VECTOR v29;

	CameraData.field_4 = 9466;
	CameraData.field_8 = 0x4000;
	ALCam_CameraManager();

	AL_NormalCameraExecutor_Load();
	AL_MinimalCreate();
	ALW_Control();
	SpawnAllChaoInGarden();
	Load_ChaoTree();
	sub_531E10();
	sub_52F2A0();
	v10 = ChaoSaveIndexThing;
	v11 = ChaoSaveIndexThing;
	if (ChaoSaveIndexThing != 1)
	{
		v11 = 0;
	}
	*(&ChaoGardensUnlocked + 12955 * v11) |= SA2BLobbyUnlocks_NeutralGarden;
	if (v10 != 1)
	{
		v10 = 0;
	}
	position.x = 72.0;
	position.y = 10.0;
	position.z = 28.0;
	*(&ChaoGardensUnlocked + 0x329B * v10) |= SA2BLobbyUnlocks_Kindergarten;
	ALO_OdekakeMachine_Load(&position, 0x6000);
	v12 = ChaoSaveIndexThing;
	if (ChaoSaveIndexThing != 1)
	{
		v12 = 0;
	}
	if ((ChaoToysUnlocked[12955 * v12] & 8) != 0)
	{
		v29.x = flt_9EBA14;
		v29.y = 5.0;
		v29.z = -46.0;
		position.x = 0.0;
		position.y = 0.0;
		position.z = 0.0;
		ALO_Ball_Load(&v29, &position);
	}
	v13 = ChaoSaveIndexThing;
	if (ChaoSaveIndexThing != 1)
	{
		v13 = 0;
	}
	if ((ChaoToysUnlocked[12955 * v13] & 1) != 0)
	{
		v29.x = 30.0;
		v29.y = 1.5;
		v29.z = -83.0;
		ALO_TVExecutor_Load(&v29);
	}
	v14 = ChaoSaveIndexThing;
	if (ChaoSaveIndexThing != 1)
	{
		v14 = 0;
	}
	if ((ChaoToysUnlocked[12955 * v14] & 4) != 0)
	{
		v29.x = 9.0;
		v29.y = 0.0;
		v29.z = -116.0;
		ALO_BoxExecutor_Load(&v29);
	}

	//LoadWaterManager(dword_1313D40);
}


/**void __cdecl Past_Garden_Manager(ObjectMaster* a1)
{
	EntityData1* data = a1->Data1.Entity;

	switch (data->Action)
	{
	case 0:
		data->Position = { 1, -64, 758 };
		data->Action++;
		break;
	case 1:
		if (IsPlayerInsideSphere(&data->Position, 100)) {
			SpawnAllElements();
			data->Action++;
		}
		break;
	}

	Manage_SoundEffect(a1);
	Animate_Water();
}*/

void __cdecl Past_Garden_Manager(ObjectMaster* a1)
{
	EntityData1* data = a1->Data1.Entity;

	switch (data->Action)
	{
	case 0:

		break;
	case 1:

		//data->Action++;
		break;
	}

	Manage_SoundEffect(a1);
	Animate_Water();
}


void init_PastGarden_Level()
{
	WriteCall((void*)0x54C80F, Load_PastGarden);

	WriteJump((void*)0x54C690, PastGarden_Display); //prevent display to run

	//ini new rot
//    WriteData((int**)0x52b8e9, &newRot);

	WriteJump(Chao_OOBLimit, Chao_OOBLimit_r); //fix OOB limit

   // WriteJump((void*)0x54C9AE, (void*)0x54cbf0);

	Init_NewStartPos();
	Init_NewTreePos();
	init_LoadSetObj();
	init_Music();

	WriteJump((void*)0x54C550, Past_Garden_Manager);

	init_ChaoFixes_Hack();
	init_WaterHack();

	//WriteJump((void*)0x54C7B0, Init_PastGarden);
}