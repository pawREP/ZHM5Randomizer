#pragma once
#include <memory>
#include <string>

namespace Config {

extern std::string base_directory;
extern std::string retail_directory;

extern std::string worldInventoryRandomizer;
extern std::string heroInventoryRandomizer;
extern std::string npcInventoryRandomizer;
extern std::string stashInventoryRandomizer;
extern bool randomizeNPCGrenades;
extern bool showDebugConsole;
extern bool enableDebugLogging;
extern bool logToFile;
extern int RNGSeed;
extern std::string randomizationScenario;

void loadConfig();
}; // namespace Config
