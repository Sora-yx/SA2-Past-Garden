#pragma once

void ReadConfig(const char* path);

extern HelperFunctions HelperFunctionsGlobal;
void init_PastGarden_Level();
void Load_skyboxModel();
void __cdecl PastGarden_Display(ObjectMaster* a1);
void init_Music();

const char PastGarden = 1;

void init_WaterHack();
void init_ChaoFixes_Hack();
void Tikal_Event(ObjectMaster* obj);
void CWE_FixesOnFrames();
void Animate_Water();
void initTimeOfDay_Hack();