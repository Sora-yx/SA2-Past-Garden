#include "pch.h"

static ModelInfo* TikalMDL = nullptr;
AnimationFile* TikalStandMotion = nullptr;
AnimationFile* TikalMoveMotion = nullptr;

NJS_TEXNAME Tikal_texname[18];
NJS_TEXLIST Tikal_texlist = { arrayptrandlengthT(Tikal_texname, Uint32) };

static ModelInfo* ChaoMDL = nullptr;
AnimationFile* ChaoStandMotion = nullptr;
AnimationFile* ChaoSingMotion = nullptr;
NJS_TEXNAME ChaoEV_texname[11];
NJS_TEXLIST ChaoEV_texlist = { arrayptrandlengthT(ChaoEV_texname, Uint32) };

NJS_VECTOR ChaoSpot[4] = { { -119, 0.9f, 347 }, { -123, 0.9f, 368 }, {-99, 0.9f , 370}, {-99, 0.9f, 352}, };

CollisionData TikalCol = { 0, (CollisionShapes)0, 0x77, 0xE0, 0x8000, {0.0}, 4.0, 0.0, 0.0, 0.0, 0, 0, 0 };

const int timerCameo = 120;
const int timerWalk = 600;
const float destTikalWalkChao = -250.0f;
bool tikalLeaving = false;

//0 = autel, 1 = ground with Chao, 2 = stair
NJS_VECTOR TikalSpot[3] = { { -11, 95, 50 }, {-88, 5.0, 360}, { -16, 12, 253} };

CollisionData ChaoEVCol = { 0, (CollisionShapes)0, 0x77, 0xE0, 0x8000, {0.0, 1.0, 0.0}, 2.0, 0.0, 0.0, 0.0, 0, 0, 0 };

enum TikalState {
	init,
	setTikal,
	standing,
	walking,
	transition,
	ending
};

enum tikalPos {
	masterEmeraldPos,
	gardenPos,
	standingStairPos,
};

const char* TikalDialogue[6] = {
	{ "\a Hello there, friend.\n How are you?"},
	{ "\a My heart has always been in the\n Master Emerald, along with Chaos."},
	{ "\a Forgive me, but I don't think\n we've ever met before...", },
	{ "\a I'm Tikal... And you are?"},
	{ "\a My true hope is that some day,\n we'll understand each other."},
	{ "\a These little creatures are too\n vulnerable without its protection."}
};

const char* getTikalDialogue(int index)
{
	if (!MainCharObj2[0])
		return NULL;

	switch (MainCharObj2[0]->CharID2)
	{
	case Characters_Sonic:
	case Characters_Amy:
	case Characters_Tails:
	case Characters_Knuckles:
	case Characters_SuperSonic:

		if (index == masterEmeraldPos)
		{
			return TikalDialogue[rand() % 2];
		}

		return TikalDialogue[0];
	case Characters_Eggman:
		return TikalDialogue[4];
	default:
		return TikalDialogue[rand() % 2 + 2];
	case Characters_Chaos:
		return TikalDialogue[5];
	}
}

void EV_Chao_Display(ObjectMaster* obj)
{
	EntityData1* data = obj->Data1.Entity;
	njControl3D_Backup();
	njControl3D_Add(NJD_CONTROL_3D_CONSTANT_MATERIAL);
	SetMaterial(1.0, 1.0, 1.0, 1.0);
	njSetTexture(&ChaoEV_texlist);
	njPushMatrix(0);
	njTranslateV(0, &data->Position);
	njRotateY(0, data->Rotation.y);

	if (!data->Index)
		DrawMotionAndObject(ChaoStandMotion->getmotion(), ChaoMDL->getmodel(), FrameCountIngame % ChaoStandMotion->getmotion()->nbFrame);
	else
		DrawMotionAndObject(ChaoSingMotion->getmotion(), ChaoMDL->getmodel(), FrameCountIngame % ChaoSingMotion->getmotion()->nbFrame);

	njPopMatrix(1u);
	ResetMaterial();
	njControl3D_Restore();
}

void EV_Chao(ObjectMaster* obj)
{
	EntityData1* data = obj->Data1.Entity;

	switch (data->Action)
	{
	case 0:
		InitCollision(obj, &ChaoEVCol, 1, 4u);
		obj->DisplaySub = EV_Chao_Display;
		data->Rotation.y = 0x8000;
		data->Position = ChaoSpot[data->field_2];
		data->Action++;
		break;
	case 1:
		if (tikalLeaving)
		{
			data->Action++;
		}
		break;
	case 2:
		DeleteObject_(obj);
		return;
	}

	if (data->Action == 1)
		AddToCollisionList(obj);
}

void Load_ChaoEvent()
{
	for (uint8_t i = 0; i < LengthOfArray(ChaoSpot); i++)
	{
		ObjectMaster* chao = LoadObject(2, "Chao_Event", EV_Chao, LoadObj_Data1);

		if (chao)
		{
			EntityData1* data = chao->Data1.Entity;
			data->field_2 = i;
			if (i < 2)
				data->Index = 0;
			else
				data->Index = 1;
		}
	}
}

void TikalChao_Delete() {
	FreeMDL(TikalMDL);
	FreeAnim(TikalStandMotion);
	FreeAnim(TikalMoveMotion);
	FreeMDL(ChaoMDL);
	FreeAnim(ChaoStandMotion);
	FreeAnim(ChaoSingMotion);
	njReleaseTexture(&ChaoEV_texlist);
	njReleaseTexture(&Tikal_texlist);
	tikalLeaving = false;
}

void Tikal_CameoDisplay(ObjectMaster* obj)
{
	EntityData1* data = obj->Data1.Entity;

	njSetTexture(&Tikal_texlist);
	njPushMatrix(0);
	njTranslateV(0, &data->Position);

	njRotateY(0, data->Rotation.y);

	if (data->Action == standing || data->Action == transition || data->Action == ending)
		DrawMotionAndObject(TikalStandMotion->getmotion(), TikalMDL->getmodel(), FrameCountIngame % TikalStandMotion->getmotion()->nbFrame);

	if (data->Action == walking)
		DrawMotionAndObject(TikalMoveMotion->getmotion(), TikalMDL->getmodel(), FrameCountIngame % TikalMoveMotion->getmotion()->nbFrame);

	njPopMatrix(1u);
}


void Tikal_Event(ObjectMaster* obj)
{
	EntityData1* data = obj->Data1.Entity;
	EntityData1* P1 = MainCharObj1[0];

	switch (data->Action)
	{
	case init:
		if (++data->field_6 == timerCameo)
		{
			int rng = rand() % 2;

			if (rng && TimeOfDay != Night)
			{
				tikalLeaving = false;
				data->Action++;
			}
			else {
				data->field_6 = 0;
			}
		}
		break;
	case setTikal:
		InitCollision(obj, &TikalCol, 1, 4u);
		data->field_6 = 0;
		data->Index = 0 + rand() % 3;
		data->Rotation.y = 0x0;
		data->Position = TikalSpot[data->Index];
		if (data->Index == 1)
		{
			Load_ChaoEvent();
		}
		obj->DisplaySub = Tikal_CameoDisplay;
		data->Action++;
		break;
	case standing:
		if (IsPlayerInsideSphere(&data->Position, 20))
		{
			if (Controllers[0].press & Buttons_Y)
			{
				MainCharObj2[0]->Speed = { 0, 0, 0 };

				DoNextAction_r(0, 9, 0);
				const char* text = getTikalDialogue(data->Index);
				if (!text)
					return;

				DrawSubtitles(1, text, 105, 1);
				data->field_6 = 0;
				data->Action = transition;
			}
		}

		if (++data->field_6 == timerWalk)
		{
			if (data->Index == gardenPos) {
				data->Action++;
				data->field_6 = 0;
			}
			else {
				data->Action = ending;
			}
		}

		break;
	case walking:

		if (data->Position.x > destTikalWalkChao)
		{
			data->Position.x -= 0.3f;
		}
		else {
			data->field_6 = 0;
			data->Rotation.y += 0x8000;
			data->Action = ending;
		}

		break;
	case transition:
		if (++data->field_6 == 80)
		{
			DoNextAction_r(0, 15, 0);
			data->Action = standing;
		}
		break;
	case ending:
		if (++data->field_6 == 120)
		{
			FreeEntityCollision(obj);
			tikalLeaving = true;
			data->field_6 = 0;
			data->Action = init;
		}
		return;
	}

	if (data->Action >= standing && data->Action != ending)
		AddToCollisionList(obj);
}

void Load_TikalAndChao()
{
	TikalMDL = LoadMDL("Tikal", ModelFormat_Chunk);
	TikalStandMotion = LoadAnim("ti_wait.nam");
	TikalMoveMotion = LoadAnim("ti_walk.nam");
	LoadTextureList("TIKAL_DC", &Tikal_texlist);

	ChaoMDL = LoadMDL("ChaoMDL", ModelFormat_Chunk);
	ChaoStandMotion = LoadAnim("chao_anim0");
	ChaoSingMotion = LoadAnim("chao_anim1");
	LoadTextureList("EV_ALIFE_DC", &ChaoEV_texlist);
}