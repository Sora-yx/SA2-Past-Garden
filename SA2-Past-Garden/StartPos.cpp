#include "pch.h"

NJS_VECTOR startPos = { 24.0f, -41.00f, 1475.0f };

NJS_VECTOR startPosLeaveCave = { 0.31f, 90.0, 75.39f };

NJS_VECTOR transporterPos = { -365, 0, 236 };
NJS_VECTOR TVPos = { 251, 0, 301 };
NJS_VECTOR BoxPos = { 207, 0, 308 };

//NJS_VECTOR startPos = { -63, 0, 387 };

NJS_VECTOR ChaoSpawnPoint_Past[16]
{
    { 125, -0.0f, 360.0f  },
    { 119, 0.0f, 390.80f},
    { 117, -2.0f, 431}, //warn
    { 144, -2.0f, 438.48},
    { 178, -2.0f, 443},
    { 199, 0, 365},
    { 253, 0, 356},
    { 272, 0, 282},
    { 168, 0, 287},
    { 110, 0, 309},
    { 27, -0, 372},
    { 13, 0, 395},
    { -3, 7, 266},
    { -11, 0, 311},
    { -53, 0, 382},
    { 268, 0, 407},
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

    //Tansporter new pos
    WriteData((float**)0x54ca00, &transporterPos.x);
    WriteData((float**)0x54ca10, &transporterPos.y);
    WriteData((float**)0x54ca1A, &transporterPos.z);
    WriteData<1>((int*)0x54ca31, 0x0); //rotation

    //ball doesn't want to spawn for some reason.
    
    //tv new pos
    WriteData((float**)0x54caba, &TVPos.x);
    WriteData((float**)0x54cac9, &TVPos.y);
    WriteData((float**)0x54cad3, &TVPos.z);

    //box new pos
    WriteData((float**)0x54cb00, &BoxPos.x);
    WriteData((float**)0x54cb0f, &BoxPos.y);
    WriteData((float**)0x54cb19, &BoxPos.z);

    Init_NewChaoStartPos();
}