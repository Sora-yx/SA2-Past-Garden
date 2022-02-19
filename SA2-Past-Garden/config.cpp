#include "pch.h"

bool quietMusic = true;
bool tikalTheme = true;

void ReadConfig(const char* path) {

	const IniFile* config = new IniFile(std::string(path) + "\\config.ini");
	quietMusic = config->getBool("Audio", "quietMusic", true);
	tikalTheme = config->getBool("Audio", "tikalTheme", true);

	delete config;

}