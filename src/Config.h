#pragma once
#include <memory>
#include <string>

namespace Config
{
	extern std::string base_directory;

	extern bool randomizeWorldInventory;
	extern bool randomizeHeroInventory;
	extern bool randomizeNPCInventory;
	extern bool randomizeStashInventory;
	extern bool randomizeNPCGrenades;
	extern bool showDebugConsole;
	extern bool enableDebugLogging;
	extern bool logToFile;
	extern int RNGSeed;
	extern std::string randomizationScenario;

	void loadConfig();
};

