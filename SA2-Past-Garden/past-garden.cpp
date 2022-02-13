#include "pch.h"
#include "Objects.h"
#include "StartPos.h"

static LandTableInfo* PastLandInfo = nullptr;
static NJS_TEXNAME PAST01_TEXNAME[83]{};
static NJS_TEXLIST PAST01_TEXLIST = { arrayptrandlength(PAST01_TEXNAME, Uint32) };
static const TexPackInfo PAST01_TEXINFO = { "PAST01_DC", &PAST01_TEXLIST };

float OOBLimit = -150.0f;

void Init_NewTreePos() {
    TreePos[3] = { 269, -1, 444 };
    TreePos[4] = { -189, 0, -381 };
    TreePos[5] = { 327, 0, 247 };
    TreePos[6] = { -429, 3, 87.0f };
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

void __cdecl Past_Garden_Main(ObjectMaster* a1)
{
    EntityData1* data;
    unsigned __int16 timer;
    int v3;
    NJS_VECTOR pos;

    pos.x = -100.0;
    pos.y = 0.0;
    data = a1->Data1.Entity;
    pos.z = -100.0;
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

    WriteJump((void*)0x54C550, Past_Garden_Main);
}