#include <Windows.h>
#include <filesystem>
#include "Config.h"

std::string Config::base_directory;

bool Config::randomizeWorldInventory;
bool Config::randomizeHeroInventory;
bool Config::randomizeNPCInventory;
bool Config::randomizeStashInventory;
bool Config::randomizeNPCGrenades;
bool Config::showDebugConsole;
bool Config::enableDebugLogging;
bool Config::logToFile;
int Config::RNGSeed;

#define LOAD_INI_ENTRY(name, cat, default_) name = GetPrivateProfileIntA(cat, #name, default_, ini_path.c_str());

void Config::loadConfig() {
	auto ini_path = Config::base_directory + "\\Retail\\ZHM5Randomizer.ini";

	LOAD_INI_ENTRY(randomizeWorldInventory, "ZHM5Randomizer", 1);
	LOAD_INI_ENTRY(randomizeHeroInventory,	"ZHM5Randomizer", 1);
	LOAD_INI_ENTRY(randomizeNPCInventory,	"ZHM5Randomizer", 1);
	LOAD_INI_ENTRY(randomizeStashInventory,	"ZHM5Randomizer", 1);
	LOAD_INI_ENTRY(randomizeNPCGrenades,	"ZHM5Randomizer", 0);
	LOAD_INI_ENTRY(RNGSeed,					"ZHM5Randomizer", 0);

	LOAD_INI_ENTRY(showDebugConsole,		"Debug", 0);
	LOAD_INI_ENTRY(enableDebugLogging,		"Debug", 0);
	LOAD_INI_ENTRY(logToFile,				"Debug", 0);

}