#pragma once

void ReadConfig(const char* path);

extern HelperFunctions HelperFunctionsGlobal;
void init_PastGarden_Level();
void Load_skyboxModel();
void __cdecl PastGarden_Display(ObjectMaster* a1);
void init_Music();