#include "pch.h"

HelperFunctions HelperFunctionsGlobal;
const char* error = "[Past Garden]: WARNING: Your version of the Mod Loader is old, the mod won't work properly.\nPlease update your Mod Loader for the best experience.";

extern "C" {

	__declspec(dllexport) void __cdecl Init(const char* path, const HelperFunctions& helperFunctions)
	{
		Sleep(10);

		HelperFunctionsGlobal = helperFunctions;

		if (HelperFunctionsGlobal.Version < 8) {
			PrintDebug(error);
			MessageBoxA(MainWindowHandle, error, "Past Garden", MB_ICONWARNING);
		}

		ReadConfig(path); //get mod settings by the user
		init_PastGarden_Level();
	}

	__declspec(dllexport) void __cdecl OnFrame() {

	}



	__declspec(dllexport) ModInfo SA2ModInfo = { ModLoaderVer };
}