#pragma once

extern bool quietMusic;
extern bool tikalTheme;

void ReadConfig(const char* path);

extern HelperFunctions HelperFunctionsGlobal;
void init_PastGarden_Level();
void Load_skyboxModel();
void __cdecl PastGarden_Display(ObjectMaster* a1);
void init_Music();
void Camera_OnFrames();

const char PastGarden = 1;
extern uint8_t TimeOfDay;

void init_WaterHack();
void init_ChaoFixes_Hack();
void Tikal_Event(ObjectMaster* obj);
void CWE_FixesOnFrames();
void Animate_Water();
void initTimeOfDay_Hack();
void PlayPastGardenMusic();

enum TimeOfDay : uint8_t
{
    Day,
    Evening,
    Night
};