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

void LoadLandTable_r()
{
    LandTableSA2BModels = 0;
    LoadLandTable("resource\\gd_PC\\past-garden.sa2lvl", &PastLandInfo, &PAST01_TEXINFO);
    Load_skyboxModel();
    return;
}

void PlayMusic_r(const char* song)
{
    PlayMusic("tical.adx");
    return;
}

static void __declspec(naked) PlayMusicASM()
{
    __asm
    {
        push edi
        call PlayMusic_r
        pop edi 
        retn
    }
}

void init_PastGarden_Level()
{
	WriteCall((void*)0x54C80F, LoadLandTable_r);

	WriteJump((void*)0x54C690, PastGarden_Display); //prevent display to run

    //ini new rot
//    WriteData((int**)0x52b8e9, &newRot);

    WriteJump(Chao_OOBLimit, Chao_OOBLimit_r); //fix OOB limit

    WriteCall((void*)0x54C984, PlayMusicASM);

   // WriteJump((void*)0x54C9AE, (void*)0x54cbf0);

    Init_NewStartPos();
    Init_NewTreePos();
    init_LoadSetObj();
}