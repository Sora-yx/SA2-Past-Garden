#include "pch.h"

NJS_VECTOR Skybox_Scale = { 1.4f, 1.4f, 1.4f };
static ModelInfo* Past_BG;
static NJS_TEXNAME PAST01BG_TEXNAME[9]{};
static NJS_TEXLIST PAST01BG_TEXLIST = { arrayptrandlength(PAST01BG_TEXNAME, Uint32) };

static ModelInfo* WaterMdl[2];

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