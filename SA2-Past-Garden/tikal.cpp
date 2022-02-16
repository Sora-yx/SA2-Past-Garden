#include "pch.h"

static ModelInfo* TikalMDL = nullptr;
AnimationFile* TikalStandMotion = nullptr;
AnimationFile* TikalMoveMotion = nullptr;

NJS_TEXNAME Tikal_texname[18];
NJS_TEXLIST Tikal_texlist = { arrayptrandlengthT(Tikal_texname, Uint32) };

CollisionData TikalCol = { 0, (CollisionShapes)0, 0x77, 0xE0, 0x8000, {0.0, 10.0, 0.0}, 7.0, 0.0, 0.0, 0.0, 0, 0, 0 };

const int timerCameo = 300;
const int timerWalk = 1800;

NJS_VECTOR TikalSpot[3] = { { -11, 95, 50 }, {-110, 5.0, 367}, { -16, 12, 253} };


enum TikalState {
	init,
	setTikal,
	standing,
	walking,
	transition
};

enum tikalPos {
	masterEmeraldPos,
	standingStairPos,
	gardenPos
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
		return TikalDialogue[rand() %2 + 2];
	case Characters_Chaos:
		return TikalDialogue[5];
	}
}

void Tikal_Delete(ObjectMaster* obj) {
	FreeMDL(TikalMDL);
	FreeAnim(TikalStandMotion);
	FreeAnim(TikalMoveMotion);
}

void Tikal_CameoDisplay(ObjectMaster* obj)
{
	EntityData1* data = obj->Data1.Entity;

	njSetTexture(&Tikal_texlist);
	njPushMatrix(0);
	njTranslateV(0, &data->Position);

	njRotateY(0, 0x8000 - data->Rotation.y);

	if (data->Action == standing || data->Action == transition)
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

			if (rng)
			{
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
		data->Position = TikalSpot[data->Index];
		obj->DisplaySub = Tikal_CameoDisplay;
		data->Action++;
		break;
	case standing:
		if (IsPlayerInsideSphere(&data->Position, 20))
		{
			if (Controllers[0].press & Buttons_Y)
			{
				data->Rotation.y = P1->Rotation.y;
				MainCharObj2[0]->Speed = { 0, 0, 0 };
				LookAt(&data->Position, &P1->Position, nullptr, &data->Rotation.y);
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
			data->Action++;
			data->field_6 = 0;
		}
		
		break;
	case walking:
		if (data->Index == masterEmeraldPos)
		{
			//data->Position.x+= 0.5f;
		}

		if (++data->field_6 == timerWalk)
		{
			data->Action = init;
			data->field_6 = 0;
		}
		break;
	case transition:
		if (++data->field_6 == 80)
		{
			DoNextAction_r(0, 15, 0);
			data->Action = standing;
		}
		break;
	}

	AddToCollisionList(obj);
}



void Load_Tikal()
{
	TikalMDL = LoadMDL("Tikal", ModelFormat_Chunk);
	TikalStandMotion = LoadAnim("ti_wait.nam");
	TikalMoveMotion = LoadAnim("ti_walk.nam");

	LoadTextureList("TIKAL_DC", &Tikal_texlist);
}