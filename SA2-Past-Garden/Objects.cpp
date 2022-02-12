#include "pch.h"

ObjectListEntry PastObjectList_list[] = {
	{ (LoadObj)2, 3, 0, 0,  RingMain, } /* "RING   " */,
	{ (LoadObj)2, 2, 0, 0,  (ObjectFuncPtr)0x6C4480, } /* "SPRING " */,
	{ (LoadObj)2, 2, 0, 0,  (ObjectFuncPtr)0x6C4E90, } /* "SPRINGB" */,
	{ (LoadObj)3, 3, 0, 0,  DashPanel_Main, } /* "O AXPNL" */,
	{ (LoadObj)3, 3, 0, 0,  nullptr, } /* "O EME P" */,
	{ (LoadObj)6, 3, 0, 0,  (ObjectFuncPtr)Switch_Main,  } /* "O SWITCH" */,
	{ (LoadObj)14, 3, 0, 0,  nullptr, } /* "CMN_DRING" */,
	{ (LoadObj)2, 3, 0, 0,  nullptr,  } /* "O BALOON" */,
	{ (LoadObj)2, 3, 0, 0,  ItemBox_Main, } /* "O ITEMBOX" */,
	{ (LoadObj)2, 2, 1, 4000000, nullptr, } /* "O JPanel" */,
	{ (LoadObj)15, 6, 0, 0, Checkpoint_Main,  } /* "O Save Point" */,
	{ (LoadObj)2, 3, 0, 0,  (ObjectFuncPtr)CCUBE,} /* "WALL   " */,
	{ (LoadObj)2, 3, 1, 640000,  nullptr,  } /* "O PALM" */,
	{ (LoadObj)2, 3, 0, 0,  nullptr, } /* "O TREE" */,
	{ (LoadObj)2, 3, 0, 0,  nullptr,  } /* "O SNAKE" */,
	{ (LoadObj)2, 3, 0, 0,  nullptr, } /* "O FALL TREE" */,
	{ (LoadObj)2, 3, 0, 0,  nullptr, } /* "O WELL" */,
	{ (LoadObj)2, 3, 0, 0,  (ObjectFuncPtr)0x55AB90,  } /* "CHAO LOBBY RETURN */,
	{ (LoadObj)2, 3, 1, 1000000,  nullptr,  } /* "O KNUTREE" */,
	{ (LoadObj)2, 3, 2, 0,  nullptr,  } /* "O PYSTAIRS" */,
	{ (LoadObj)2, 3, 2, 0,  nullptr, } /* "O BIGSTAIRS" */,
	{ (LoadObj)2, 3, 1, 360000,  nullptr, } /* "O KNUDOOR" */,
	{ (LoadObj)2, 3, 0, 0, nullptr, } /* "O SHIDA" */,
	{ (LoadObj)2, 3, 0, 0, nullptr, } /* "O SOTETSU" */,
	{ (LoadObj)2, 3, 2, 0, nullptr,  } /* "O SMOKE" */,
	{ (LoadObj)2, 3, 1, 640000, nullptr } /* "O SOUND" */,
	{ (LoadObj)2, 3, 1, 490000, nullptr,  } /* "O KNULAMP" */,
	{ (LoadObj)2, 3, 1, 1000000,  nullptr,  } /* "O M EMERALD" */,
	{ (LoadObj)2, 3, 1, 1000000,  nullptr, } /* "O C EMERALD" */,
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

	//LoadTextureList("objtex_common", texlist_objtex_common);
	return LoadLevelLayout(&PastObjectList, "SetPast.bin", "SET0048_2P_U.bin");
}

void init_LoadSetObj() {
	WriteCall((void*)0x54C82C, LoadSetObject_r);
}