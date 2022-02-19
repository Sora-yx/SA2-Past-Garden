#include "pch.h"

bool playedOnce = false;

void MusicEvent(ObjectMaster* obj)
{
	EntityData1* data = obj->Data1.Entity;

	if (GameState != GameStates_Ingame && !playedOnce) {
		StopMusic(); //sa2 spam ResetMusic function so we force it to be quiet for a bit.
		return;
	}

	switch (data->Action)
	{
	case 0:
		data->Position = { 1, -64, 758 };
		data->Action++;

		break;
	case 1:
		if (IsPlayerInsideSphere(&data->Position, 100))
		{
			if (TimeOfDay != Night)
			{
				PlayMusic("tical.adx");
				ResetMusic();
				playedOnce = true;
				data->Action++;
			}
			else {
				data->Action = 3;
			}
		}
		break;
	case 2:
		if (TimeOfDay == Night)
		{
			StopMusic();
			data->Action++;
		}
		break;
	case 3:
		if (TimeOfDay != Night)
		{
			PlayMusic("tical.adx");
			ResetMusic();
			data->Action--;
		}
		break;
	default:
		DeleteObject_(obj);
		break;
	}
}


void Stop_Music_r()
{
	StopMusic();

	if (playedOnce && TimeOfDay != Night)
	{
		PlayMusic("tical.adx");
		ResetMusic();
		return;
	}

	LoadObject(2, "EventPast_MusicManager", MusicEvent, LoadObj_Data1);
	return;
}

void init_Music()
{

	//prevent the game to switch music, we will manually start the music later
	WriteData<5>((int*)0x54C984, 0x90);
	WriteCall((void*)0x54C989, Stop_Music_r);
}