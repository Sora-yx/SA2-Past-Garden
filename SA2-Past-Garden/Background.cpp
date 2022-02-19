#include "pch.h"

NJS_VECTOR Skybox_Scale = { 1.4f, 1.4f, 1.4f };
static ModelInfo* Past_BG;
static NJS_TEXNAME PAST01BG_TEXNAME[9]{};
static NJS_TEXLIST PAST01BG_TEXLIST = { arrayptrandlength(PAST01BG_TEXNAME, Uint32) };

static ModelInfo* WaterMdl[2];
static Trampoline* LoadLighting_t = nullptr;

enum TimeOfDay : uint8_t
{
    Day,
    Evening,
    Night
};

uint8_t TimeOfDay = Day;

void SetTimeOfDay(uint8_t time)
{
    TimeOfDay = time;

    switch (TimeOfDay)
    {
    case Day:
    default:
        PAST01BG_TEXLIST.textures[0].texaddr = PAST01BG_TEXLIST.textures[0].texaddr;
        PAST01BG_TEXLIST.textures[1].texaddr = PAST01BG_TEXLIST.textures[1].texaddr;
        PAST01BG_TEXLIST.textures[2].texaddr = PAST01BG_TEXLIST.textures[2].texaddr;
        break;
    case Evening:
        PAST01BG_TEXLIST.textures[0].texaddr = PAST01BG_TEXLIST.textures[6].texaddr;
        PAST01BG_TEXLIST.textures[1].texaddr = PAST01BG_TEXLIST.textures[7].texaddr;
        PAST01BG_TEXLIST.textures[2].texaddr = PAST01BG_TEXLIST.textures[8].texaddr;
        break;
    case Night:
        PAST01BG_TEXLIST.textures[0].texaddr = PAST01BG_TEXLIST.textures[3].texaddr;
        PAST01BG_TEXLIST.textures[1].texaddr = PAST01BG_TEXLIST.textures[4].texaddr;
        PAST01BG_TEXLIST.textures[2].texaddr = PAST01BG_TEXLIST.textures[5].texaddr;
        break;
    }
}

static inline Sint32 LoadStageLight_origin(const char* filename)
{
    const auto target = LoadLighting_t->Target();

    signed int result;
    __asm
    {
        mov ecx, [filename]
        call target
        mov result, eax
    }
    return result;
}

Sint32 LoadStageLight_r(const char* filename)
{
    std::string text = filename;

    if (CurrentLevel == LevelIDs_ChaoWorld && CurrentChaoArea == PastGarden)
    {
        if (text == "stg00_eve_light.bin")
        {
            SetTimeOfDay(Evening);
        }
        else if (text == "stg00_ngt_light.bin")
        {
            SetTimeOfDay(Night);
        }
        else if (text == "stg00_cld_light.bin")
        {
            SetTimeOfDay(Day);
        }
    }

    return LoadStageLight_origin(filename);
}


static void __declspec(naked) LoadLStageLightASM()
{
    __asm
    {
        push ecx
        call LoadStageLight_r
        pop ecx 
        retn
    }
}


int countTexSea = 73;
int countTexFountain = 59;

extern NJS_TEXLIST PAST01_TEXLIST;

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


void DrawWater(ObjectMaster* obj, char id)
{
    EntityData1* data = obj->Data1.Entity;
    NJS_VECTOR pos = { WaterMdl[id]->getmodel()->pos[0], WaterMdl[id]->getmodel()->pos[1], WaterMdl[id]->getmodel()->pos[2] };

    njSetTexture(CurrentLandTable->TextureList);
    njPushMatrix(0);
    njTranslateV(0, &pos);
    DrawObject(WaterMdl[id]->getmodel());
    njPopMatrix(1u);

}

void __cdecl PastGarden_Display(ObjectMaster* a1)
{

    EntityData1* data; // eax

    data = a1->Data1.Entity;

    njControl3D_Backup();
    njControl3D_Add(NJD_CONTROL_3D_NO_CLIP_CHECK);
    njControl3D_Remove(NJD_CONTROL_3D_DEPTH_QUEUE);
    njPushMatrix(0);
    njTranslate(0, CameraData.Position.x, 0.0, CameraData.Position.z);
    njScale(0, 2.0, 2.0, 2.0);
    njSetTexture(&PAST01BG_TEXLIST);
    njScaleV_(&Skybox_Scale);
    DrawObject(Past_BG->getmodel());
    njScale(0, 1.0, 1.0, 1.0);
    njPopMatrix(1u);
    njControl3D_Restore();

    for (int i = 0; i < LengthOfArray(WaterMdl); i++)
    {
        DrawWater(a1, i);
    }
}

void Load_skyboxModel()
{
    LoadTextureList("MR_SKY00_DC", &PAST01BG_TEXLIST);
    Past_BG = LoadMDL("past-skybox", ModelFormat_Chunk);

    for (size_t i = 0; i < LengthOfArray(WaterMdl); i++) {
        std::string str = "Water" + std::to_string(i);
        WaterMdl[i] = LoadMDL(str.c_str(), ModelFormat_Chunk);
    }
}

void initTimeOfDay_Hack()
{
    LoadLighting_t = new Trampoline((int)0x6C3AE0, (int)0x6C3AE8, LoadLStageLightASM);
}