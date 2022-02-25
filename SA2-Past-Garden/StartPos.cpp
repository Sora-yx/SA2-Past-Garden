#include "pch.h"

NJS_VECTOR startPos = { 24.0f, -41.00f, 1475.0f };

NJS_VECTOR startPosLeaveCave = { 0.31f, 90.0, 75.39f };

NJS_VECTOR transporterPos = { -365, 0, 236 };
NJS_VECTOR TVPos = { 251, 0, 301 };
NJS_VECTOR BoxPos = { 207, 0, 308 };

//NJS_VECTOR startPos = { -63, 0, 387 };

NJS_VECTOR ChaoSpawnPoint_Past[16]
{
    { 126, -1.0f, 386.0f  },
    { 200, -1.0f, 377.80f},
    { 167, -1.0f, 291},
    { -6, -1.0f, 332.48},
    { 65, -1.0f, 369},
    { -4, 6, 245},
    { 35, -1, 333},
    { 253, -1, 342},
    { 240, -2, 443},
    { 170, -7, 477},
    { 204, -6, 476},
    { 180, -14, 520},
    { 90, -13, 510},
    { 357, -1, 124},
    { 377, -1, 64},
    { 120 -12, 506},
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