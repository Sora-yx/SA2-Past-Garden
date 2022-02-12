#include "pch.h"

NJS_VECTOR startPos = { 219.0f, -30.00f, 1777.0f };

NJS_VECTOR startPosLeaveCave = { 0.31f, 90.0, 75.39f };

//NJS_VECTOR startPos = { -63, 0, 387 };

NJS_VECTOR ChaoSpawnPoint_Past[16]
{
    { 0, 90, 53  },
    { 225, 0, 200 },
    { -163, 0, 371 },
    { -349, 0, 185 },
    { -361, 0, -131},
    { 254, -1, 447 },
    { 298, 0, 315.99},
    { 358, 0, -126},
    { 286, 0, -313},
    { 41, 0, -296},
    {-48, 90, -19},
    { -320, 1, 472},
    { -63, 0, 387},
    { -5, -0, 386},
    { 49, -0, 393},
    {  0, 7, 250},
};

void Init_NewChaoStartPos()
{
    for (int i = 0; i < 16; i++) {

        ProbablyChaoSpawnPoints[i] = ChaoSpawnPoint_Past[i];
    }
}

void Init_NewStartPos()
{
    //init new start pos 
    WriteData((float**)0x52b8ce, &startPos.x);
    WriteData((float**)0x52b8d6, &startPos.y);
    WriteData((float**)0x52b8df, &startPos.z);   
    
    //start pos after leaving race cave
    WriteData((float**)0x52b882, &startPosLeaveCave.x);
    WriteData((float**)0x52b88A, &startPosLeaveCave.y);
    WriteData((float**)0x52b893, &startPosLeaveCave.z);

    Init_NewChaoStartPos();
}