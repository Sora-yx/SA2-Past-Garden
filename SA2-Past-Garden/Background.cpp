#include "pch.h"

/** PastSkyboxScale_0 = {{1, 1, 1}, {1, 1, 1}, {1, 1, 1}};
 PastSkyboxScale_1 = { { 1.4f, 1.4f, 1.4f }, { 1, 1, 1 }, { 1, 1, 1 } };
 PastSkyboxScale_2 = { { 1.45f, 1.48f, 1.45f }, { 1.45f, 1.48f, 1.45f }, { 1.45f, 1.48f, 1.45f } };*/

NJS_VECTOR Skybox_Scale = { 1.4f, 1.4f, 1.4f };
static ModelInfo* Past_BG;
static NJS_TEXNAME PAST01BG_TEXNAME[9]{};
static NJS_TEXLIST PAST01BG_TEXLIST = { arrayptrandlength(PAST01BG_TEXNAME, Uint32) };

float __cdecl VectorMaxAbs(NJS_VECTOR* y)
{
    double result; // st7
    float x; // [esp+0h] [ebp-4h]
    float v4; // [esp+8h] [ebp+4h]

    x = fabs(y->x);
    v4 = fabs(y->y);
    result = fabs(y->z);
    if (x <= (double)v4)
    {
        if (v4 > result)
        {
            result = v4;
        }
    }
    else if (x > result)
    {
        result = x;
    }
    return result;
}

#pragma pack(push, 8)
union ModelPointers
{
    NJS_MODEL* basic;
    NJS_CNK_MODEL* chunk;
    SA2B_Model* sa2b;
};
#pragma pack(pop)


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
}


void Load_skyboxModel()
{
    LoadTextureList("MR_SKY00_DC", &PAST01BG_TEXLIST);
    Past_BG = LoadMDL("past-skybox", ModelFormat_Chunk);
}