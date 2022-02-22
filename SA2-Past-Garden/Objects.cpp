#include "pch.h"

static NJS_TEXNAME PAST01Obj_TEXNAME[50]{};
NJS_TEXLIST PAST01Obj_TEXLIST = { arrayptrandlength(PAST01Obj_TEXNAME, Uint32) };

static NJS_TEXNAME WarpObj_TEXNAME[10]{};
static NJS_TEXLIST WarpObj_TEXLIST = { arrayptrandlength(WarpObj_TEXNAME, Uint32) };

static ModelInfo* ChaosEmerald[7];
static ModelInfo* MasterEmerald;
static ModelInfo* KnuDoor;
static ModelInfo* Palm;
static ModelInfo* Tree;
static ModelInfo* EggWarp;
static ModelInfo* WarpEffect;

CollisionData ME_Col = { 0, (CollisionShapes)0, 0x77, 0xE0, 0x8000, {0.0, 10.0, 0.0}, 20.0, 0.0, 0.0, 0.0, 0, 0, 0 };
CollisionData OPalmCol = { 0, (CollisionShapes)1, 0x77, 0xE0, 0x2000, {0.0, 40.0, 0.0}, 10.0, 40.0, 0.0, 0.0, 0, 0, 0 };
CollisionData OTreeCol = { 0,  (CollisionShapes)1, 0x77, 0xE0, 0x2000, {0.0, 25.0, 0.0}, 8.0, 25.0, 0.0, 0.0, 0, 0, 0 };

CollisionData EggWarpCol[] = {
	{ 0, (CollisionShapes)1, 0x77, 0, 0, {0}, 14.7, 1.4, 0.0, 0.0, 0, 0, 0 },
	{ 0, (CollisionShapes)1, 0x77, 0, 0, {0}, 12.7, 2.8, 0.0, 0.0, 0, 0, 0},
	{ 0, (CollisionShapes)1, 0x77, 0, 0, {0}, 8.5, 4.5, 0.0, 0.0, 0, 0, 0 },
	{ 0, (CollisionShapes)0, 0x70, 0, 0x2400, {0.0, 4.8000002, 0.0}, 3.7, 0.0, 0.0, 0.0, 0, 0, 0},
};


void FreeObjectsGarden()
{
	for (uint8_t i = 0; i < LengthOfArray(ChaosEmerald); i++) {
		FreeMDL(ChaosEmerald[i]);
	}

	FreeMDL(MasterEmerald);
	FreeMDL(KnuDoor);
	FreeMDL(Palm);
	FreeMDL(Tree);
	FreeMDL(EggWarp);
	FreeMDL(WarpEffect);

	njReleaseTexture(&PAST01Obj_TEXLIST);
	njReleaseTexture(&WarpObj_TEXLIST);

	return;
}

void WarpEffect_Display(ObjectMaster* obj)
{
	Angle v1; // eax
	float XScale; // [esp+0h] [ebp-Ch]
	float YScale; // [esp+4h] [ebp-8h]
	float r; // [esp+8h] [ebp-4h]

	EntityData1* data = obj->Data1.Entity;
	XScale = data->Scale.x * 0.69999999;
	r = data->Scale.z;
	YScale = data->Scale.y * 4.5;
	njSetTexture(&WarpObj_TEXLIST);
	WarpEffect->getmodel()->evalflags |= NJD_FLAG_USE_ALPHA;
	njPushMatrix(0);
	SetMaterial(r, r, r, r);
	njTranslateV(0, &data->Position);

	njRotateY(0, data->Rotation.y);

	njScale(0, XScale, YScale, XScale);
	if (XScale > (double)YScale)
	{
		YScale = XScale;
	}
	DrawObject(WarpEffect->getmodel());
	njPopMatrix(1u);
	WarpEffect->getmodel()->evalflags &= ~NJD_FLAG_USE_ALPHA;
	ResetMaterial();
}

void __cdecl WarpEffect_Main(ObjectMaster* obj)
{
	EntityData1* data; // esi
	double scaleX; // st7
	Angle rotX;
	double scaleY; // st7
	double scaleZ; // st6

	data = obj->Data1.Entity;

	if (!data->Action)
	{
		scaleX = data->Scale.x - 0.0020000001;
		data->Rotation.y += 384;
		data->Scale.x = scaleX;
		scaleY = njSin(data->Rotation.x);
		rotX = data->Rotation.x;
		data->Scale.y = scaleY;
		scaleZ = data->Scale.z - 0.018181818;
		data->Rotation.x = rotX + 546;
		data->Scale.z = scaleZ;
		if (rotX + 546 >= 0x4000)
		{
			data->Position.y = (2.0 - scaleY) * 9.0 + data->field_6;
		}
		else
		{
			data->Position.y = scaleY * 9.0 + data->field_6;
		}
	}
	if (data->Rotation.x > 0x8000)
	{
		DeleteObject_(obj);
	}
}
void __cdecl LoadWarpEffect(ObjectMaster* a1)
{

	EntityData1* data = a1->Data1.Entity;
	data->Scale = { 1.0, 0.0, 0.0 };
	data->Rotation.x = 0;
	data->Rotation.z = 0;
	a1->MainSub = WarpEffect_Main;
	a1->DisplaySub = WarpEffect_Display;
}

void EggWarp_Display(ObjectMaster* a2)
{

	Angle v1; // eax
	float XDist; // [esp+0h] [ebp-10h]
	float YDist; // [esp+4h] [ebp-Ch]
	float ZDist; // [esp+8h] [ebp-8h]

	EntityData1* data = a2->Data1.Entity;
	njSetTexture(&WarpObj_TEXLIST);
	njPushMatrix(0);
	ZDist = njSin(data->Rotation.z) * 0.079999998 + data->Position.z;
	YDist = njSin(data->Rotation.x + data->Rotation.z) * 0.050000001 + data->Position.y;
	XDist = njSin(data->Rotation.x) * 0.029999999 + data->Position.x;
	njTranslate(0, XDist, YDist, ZDist);
	v1 = data->Rotation.y;

	njRotateY(0, (unsigned __int16)v1);
	
	njScale(0, 0.69999999, 0.69999999, 0.69999999);

	DrawObject(EggWarp->getmodel());
	njPopMatrix(1u);

}


void EggWarp_Main(ObjectMaster* obj)
{
	EntityData1* data = obj->Data1.Entity;
	char pID = 0;
	ObjectMaster* warp;
	ObjectMaster* raceEntry;
	int timer;

	switch (data->Action)
	{
	case 0:
		InitCollision(obj, EggWarpCol, 4, 4u);
		obj->DisplaySub = EggWarp_Display;
		data->Action++;
		break;
	case 1:
		if (GetCollidingPlayer(obj))
		{
			if (++data->field_6 > 0x32u)
			{
				MainCharObj1[pID]->Action = 0;
				data->Action++;
				data->field_6 = 0;
				ControllerEnabled[pID] = 0;
			}
		}
		else {
			data->field_6 = 0;
		}
		break;
	case 2:
		ResetPlayerSpeed(0);
		data->field_6 = 0;
		data->Action++;
		break;
	case 3:

		warp = LoadObject(3, "WarpEffect", LoadWarpEffect, LoadObj_Data1 | LoadObj_Data2);
		if (warp)
		{
			EntityData1* warpData = warp->Data1.Entity;
			warpData->Position = data->Position;
			warpData->field_6 = data->Position.y + 2.0;
			timer = warpData->field_6 - 10;
		}

		if (++data->field_6 == 80)
		{
			data->Action++;
			data->field_6 = 0;
		}
		break;
	case 4:
		raceEntry = LoadObject(2, "raceEntry", (ObjectFuncPtr)0x55AC60, LoadObj_Data1 | LoadObj_Data2);
		raceEntry->Data1.Entity->Scale = data->Scale;
		raceEntry->Data1.Entity->Rotation = data->Rotation;
		raceEntry->Data1.Entity->Position = data->Position;
		data->Action++;
		break;
	}

	AddToCollisionList(obj);
}

void __cdecl OTree_0(ObjectMaster* obj)
{
	EntityData1* data;

	if (ClipSetObject(obj))
		return;

	data = obj->Data1.Entity;

	if (data->Action == 0)
	{
		obj->field_4C = Tree->getmodel();
		obj->DisplaySub = ObjectGeneric_DisplayV;
		InitCollision(obj, &OTreeCol, 1, 4u);
		data->Action++;
	}

	AddToCollisionList(obj);
}

void __cdecl OKnudoor(ObjectMaster* obj)
{
	EntityData1* data;

	data = obj->Data1.Entity;

	if (!ClipSetObject(obj))
	{
		if (!data->Action)
		{
			obj->field_4C = KnuDoor->getmodel();
			obj->DisplaySub = ObjectGeneric_DisplayV;
			data->Action++;
		}
	}
}

void __cdecl OPalm_0(ObjectMaster* obj)
{
	EntityData1* data;
	double v2;

	if (ClipSetObject(obj))
		return;

	data = obj->Data1.Entity;

	if (data->Action == 0)
	{
		obj->field_4C = Palm->getmodel();
		obj->DisplaySub = ObjectGeneric_DisplayV;
		InitCollision(obj, &OPalmCol, 1, 4u);
		data->Action++;
	}

	AddToCollisionList(obj);
}

void __cdecl MasterEmePast_Display(ObjectMaster* obj)
{

	EntityData1* data = obj->Data1.Entity;
	njSetTexture(&PAST01Obj_TEXLIST);
	njPushMatrix(0);
	njTranslate(0, data->Position.x, data->Position.y, data->Position.z);
	njRotateY(0, data->Rotation.y);

	DrawObject(MasterEmerald->getmodel());
	njPopMatrix(1u);
}

void __cdecl MasterEmePast_Main(ObjectMaster* obj)
{
	EntityData1* data = obj->Data1.Entity;

	switch (data->Action)
	{
	case 0:
		InitCollision(obj, &ME_Col, 1, 4u);
		obj->DisplaySub = MasterEmePast_Display;
		data->Action++;
		break;
	case 1:
		data->Rotation.y += 128;
		AddToCollisionList(obj);
		break;
	}
}

void __cdecl ChaosEmePast_Display(ObjectMaster* obj)
{
	EntityData1* data;

	data = obj->Data1.Entity;

	njSetTexture(&PAST01Obj_TEXLIST);
	njPushMatrix(0);
	njTranslate(0, data->Position.x, data->Position.y, data->Position.z);
	njRotateY(0, data->Rotation.y);
	DrawObject(ChaosEmerald[data->NextAction]->getmodel());
	njPopMatrix(1u);
}

void __cdecl ChaosEmePast_Main(ObjectMaster* obj)
{
	EntityData1* data;
	char curColor;

	data = obj->Data1.Entity;

	if (data->Action == 0)
	{
		curColor = data->Rotation.x;
		data->NextAction = curColor;
		if (curColor < 0 || curColor >= 7)
		{
			data->NextAction = 0;
		}
		obj->DisplaySub = ChaosEmePast_Display;
		data->Action++;
	}
	else {
		data->Rotation.y += 128;
	}
}

ObjectListEntry PastObjectList_list[] = {
	{ (LoadObj)2, 3, 0, 0,  nullptr, } /* "RING   " */,
	{ (LoadObj)2, 2, 0, 0,  (ObjectFuncPtr)0x6C4480, } /* "SPRING " */,
	{ (LoadObj)2, 2, 0, 0,  (ObjectFuncPtr)0x6C4E90, } /* "SPRINGB" */,
	{ (LoadObj)3, 3, 0, 0,  nullptr, } /* "O AXPNL" */,
	{ (LoadObj)3, 3, 0, 0,  nullptr, } /* "O EME P" */,
	{ (LoadObj)6, 3, 0, 0,  nullptr,  } /* "O SWITCH" */,
	{ (LoadObj)14, 3, 0, 0,  nullptr, } /* "CMN_DRING" */,
	{ (LoadObj)2, 3, 0, 0,  nullptr,  } /* "O BALOON" */,
	{ (LoadObj)2, 3, 0, 0,  nullptr, } /* "O ITEMBOX" */,
	{ (LoadObj)2, 2, 1, 4000000, nullptr, } /* "O JPanel" */,
	{ (LoadObj)15, 6, 0, 0, nullptr,  } /* "O Save Point" */,
	{ (LoadObj)2, 3, 0, 0,  (ObjectFuncPtr)CCUBE,} /* "WALL   " */,
	{ (LoadObj)2, 2, 1, 1000000,  OPalm_0,  } /* "O PALM" */,
	{ (LoadObj)2, 3, 1, 1000000, OTree_0, } /* "O TREE" */,
	{ (LoadObj)2, 3, 0, 0,  nullptr,  } /* "O SNAKE" */,
	{ (LoadObj)2, 3, 0, 0,  nullptr, } /* "O FALL TREE" */,
	{ (LoadObj)2, 3, 0, 0,  nullptr, } /* "O WELL" */,
	{ (LoadObj)2, 3, 0, 0,  (ObjectFuncPtr)0x55AB90,  } /* "CHAO LOBBY RETURN */,
	{ (LoadObj)2, 3, 1, 1000000,  nullptr,  } /* "O KNUTREE" */,
	{ (LoadObj)2, 3, 2, 0,  nullptr,  } /* "O PYSTAIRS" */,
	{ (LoadObj)2, 3, 2, 0,  nullptr, } /* "O BIGSTAIRS" */,
	{ (LoadObj)2, 3, 1, 360000,  OKnudoor, } /* "O KNUDOOR" */,
	{ (LoadObj)2, 3, 0, 0, nullptr, } /* "O SHIDA" */,
	{ (LoadObj)2, 3, 0, 0, nullptr, } /* "O SOTETSU" */,
	{ (LoadObj)2, 3, 2, 0, nullptr,  } /* "O SMOKE" */,
	{ (LoadObj)2, 3, 1, 640000, nullptr } /* "O SOUND" */,
	{ (LoadObj)2, 3, 1, 490000, nullptr,  } /* "O KNULAMP" */,
	{ (LoadObj)2, 3, 1, 1000000,  MasterEmePast_Main,  } /* "O M EMERALD" */,
	{ (LoadObj)2, 3, 1, 1000000,  ChaosEmePast_Main, } /* "O C EMERALD" */,
	{ (LoadObj)2, 3, 1, 1000000,  nullptr,  } /* "O FIRE" */,
	{ (LoadObj)2, 3, 1, 1000000,  nullptr, } /* "O BURNING" */,
	{ (LoadObj)2, 3, 1, 1000000,  nullptr,  } /* "O FIRESMOKE" */,
	{ (LoadObj)15, 3, 0, 0,  nullptr, } /* "O LevelItem" */,
	{ (LoadObj)2, 2, 0, 0, (ObjectFuncPtr)SPHERE,  } /* "C SPHERE" */,
	{ (LoadObj)2, 2, 0, 0, (ObjectFuncPtr)CCYL,  } /* "C CYLINDER" */,
	{ (LoadObj)2, 2, 0, 0, (ObjectFuncPtr)CCUBE,  } /* "C CUBE" */,
	{ (LoadObj)2, 3, 0, 0, (ObjectFuncPtr)CCUBE, } /* "WALL   " */,
	{ (LoadObj)2, 2, 0, 0, nullptr,  } /* "OTTOTTO" */,
	{ (LoadObj)2, 3, 0, 0, nullptr,  } /* "O PEOPLE" */,
	{ (LoadObj)10, 3, 0, 0,  nullptr,  } /* "CMN KUSA" */,
	{ (LoadObj)2, 2, 0, 0, EggWarp_Main,  (char*)"ENTRANCE" } /* "ENTRANCE" */,
	{ (LoadObj)2, 2, 0, 0, C_CLIMB, (char*)"C_CLIMB"}
};
//(ObjectFuncPtr)0x55AC60

ObjectListHead PastObjectList = { arraylengthandptr(PastObjectList_list) };

void LoadLevelLayout(ObjectListHead* objlist, const char* s, const char* u)
{
	void* setfile = LoadSETFile(2048, (char*)s, (char*)u);
	LoadSetObject(objlist, setfile);
}

void Load_TikalAndChao();

void LoadSetObject_r(ObjectListHead* list, SETEntry* setfile) {

	LoadTextureList("OBJ_PAST_DC", &PAST01Obj_TEXLIST);
	LoadTextureList("EC_ALIFE_DC", &WarpObj_TEXLIST);

	for (size_t i = 0; i < LengthOfArray(ChaosEmerald); i++) {
		std::string str = "ChaosEmerald" + std::to_string(i);
		ChaosEmerald[i] = LoadMDL(str.c_str(), ModelFormat_Chunk);
	}

	MasterEmerald = LoadMDL("MasterEmerald", ModelFormat_Chunk);
	KnuDoor = LoadMDL("Knudoor", ModelFormat_Chunk);
	Palm = LoadMDL("Palm", ModelFormat_Chunk);
	Tree = LoadMDL("tree", ModelFormat_Chunk);
	EggWarp = LoadMDL("EggWarp", ModelFormat_Chunk);
	WarpEffect = LoadMDL("WarpEffect", ModelFormat_Chunk);

	Load_TikalAndChao();

	return LoadLevelLayout(&PastObjectList, "SetPast.bin", "SET0048_2P_U.bin");
}

void init_LoadSetObj() {
	WriteCall((void*)0x54C82C, LoadSetObject_r);
}