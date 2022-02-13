#include "pch.h"

static NJS_TEXNAME PAST01Obj_TEXNAME[50]{};
NJS_TEXLIST PAST01Obj_TEXLIST = { arrayptrandlength(PAST01Obj_TEXNAME, Uint32) };

static ModelInfo* ChaosEmerald[7];
static ModelInfo* MasterEmerald;
static ModelInfo* KnuDoor;
static ModelInfo* Palm;
static ModelInfo* Tree;

CollisionData ME_Col = { 0, (CollisionShapes)0, 0x77, 0xE0, 0x8000, {0.0, 10.0, 0.0}, 20.0, 0.0, 0.0, 0.0, 0, 0, 0 };
CollisionData OPalmCol = { 0, (CollisionShapes)1, 0x77, 0xE0, 0x2000, {0.0, 40.0, 0.0}, 10.0, 40.0, 0.0, 0.0, 0, 0, 0 };
CollisionData OTreeCol = { 0,  (CollisionShapes)1, 0x77, 0xE0, 0x2000, {0.0, 25.0, 0.0}, 8.0, 25.0, 0.0, 0.0, 0, 0, 0 };

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
	{ (LoadObj)2, 3, 1, 640000,  OPalm_0,  } /* "O PALM" */,
	{ (LoadObj)2, 3, 0, 0, OTree_0, } /* "O TREE" */,
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
	{ (LoadObj)2, 2, 0, 0, (ObjectFuncPtr)0x55AC60, (char*)"ENTRANCE" } /* "ENTRANCE" */,
};

ObjectListHead PastObjectList = { arraylengthandptr(PastObjectList_list) };

void LoadLevelLayout(ObjectListHead* objlist, const char* s, const char* u)
{
	void* setfile = LoadSETFile(2048, (char*)s, (char*)u);
	LoadSetObject(objlist, setfile);
}

void LoadSetObject_r(ObjectListHead* list, SETEntry* setfile) {

	LoadTextureList("OBJ_PAST_DC", &PAST01Obj_TEXLIST);

	for (size_t i = 0; i < LengthOfArray(ChaosEmerald); i++) {
		std::string str = "ChaosEmerald" + std::to_string(i);
		ChaosEmerald[i] = LoadMDL(str.c_str(), ModelFormat_Chunk);
	}

	MasterEmerald = LoadMDL("MasterEmerald", ModelFormat_Chunk);
	KnuDoor = LoadMDL("Knudoor", ModelFormat_Chunk);
	Palm = LoadMDL("Palm", ModelFormat_Chunk);
	Tree = LoadMDL("tree", ModelFormat_Chunk);

	return LoadLevelLayout(&PastObjectList, "SetPast.bin", "SET0048_2P_U.bin");
}

void init_LoadSetObj() {
	WriteCall((void*)0x54C82C, LoadSetObject_r);
}