#include "pch.h"
#include "FastFunctionHook.hpp"

float waterHeight = -1000.2f;
FastFunctionHook<signed int, ObjectMaster*> Chao_DetectWater_h(0x561630);

//Hack water chao functions to make them working properly in the Past Garden 
//Credits: Kell (Chao Gameplay)

float GetWaterHeight(ChaoData1* chaodata1) {
    NJS_VECTOR pos = { chaodata1->entity.Position.x, chaodata1->entity.Position.y - 1.0f, chaodata1->entity.Position.z };
    CharSurfaceInfo surfaceinfo;

    ListGroundForCollision(pos.x, pos.y, pos.z, 200.0f);
    GetCharacterSurfaceInfo(&pos, &surfaceinfo);

    if (surfaceinfo.TopSurface & SurfaceFlag_Water)
    {
        return surfaceinfo.TopSurfaceDist;
    }
    else
    {
        return -10000000.0f;
    }
}

signed int __cdecl Chao_DetectWater_r(ObjectMaster* obj) {

    ChaoData1* data1 = (ChaoData1*)obj->Data1.Chao;

    if (CurrentLevel == LevelIDs_ChaoWorld && CurrentChaoArea != PastGarden)
    {
        return Chao_DetectWater_h.Original(obj);
    }
    else
    {
        ChaoData1* data = obj->Data1.Chao;
        ChaoData2* data2 = (ChaoData2*)obj->EntityData2;

        if (data->entity.Status >= 0)
        {
            data2->WaterHeight = GetWaterHeight(data);

            if (data->entity.Position.y + 2.0f >= data2->WaterHeight)
            {
                data->ChaoBehaviourInfo.CurrentActionInfo.field_0 &= 0xFFFAu;
                return 0;
            }
            else
            {
                if (!(data->ChaoBehaviourInfo.CurrentActionInfo.field_0 & 1))
                {
                    data->ChaoBehaviourInfo.CurrentActionInfo.field_0 = data->ChaoBehaviourInfo.CurrentActionInfo.field_0 | 1;
                    Play3DSound_Pos(0x1020, &obj->Data1.Entity->Position, 0, 0, 0);
                    RunChaoBehaviour(obj, (void*)0x562330, -1);
                }

                if (data2->float4 < 0.0)
                {
                    data2->float4 = data2->float4 * 0.1000000014901161;
                }

                data->ChaoBehaviourInfo.CurrentActionInfo.field_0 |= 4u;

                return 1;
            }
        }
        else
        {
            data->ChaoBehaviourInfo.CurrentActionInfo.field_0 &= 0xFFFEu;
            return 0;
        }
    }
}


void init_WaterHack()
{
	//Chao detects water with position Y instead of collision, this makes them swim in the wrong place;
	//this prevent that pos Y check to work.

	WriteData((float**)0x56168d, &waterHeight);
    Chao_DetectWater_h.Hook(Chao_DetectWater_r);
}