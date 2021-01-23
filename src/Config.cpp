#include "Config.h"
#include <Windows.h>
#include <filesystem>

std::string Config::base_directory;

std::string Config::worldInventoryRandomizer;
std::string Config::heroInventoryRandomizer;
std::string Config::npcInventoryRandomizer;
std::string Config::stashInventoryRandomizer;
bool Config::randomizeNPCGrenades;
bool Config::showDebugConsole;
bool Config::enableDebugLogging;
bool Config::logToFile;
int Config::RNGSeed;
std::string Config::randomizationScenario;

constexpr auto iniStringBufSize = 32;
constexpr const char* iniMainCategory = "ZHM5Randomizer";

#define LOAD_INI_ENTRY(name, cat, default_) \
    name = GetPrivateProfileIntA(cat, #name, default_, ini_path.c_str());
#define LOAD_INI_STRING_ENTRY(name, cat, default_)                                                    \
    char strbuf_##name[iniStringBufSize];                                                             \
    GetPrivateProfileString(cat, #name, default_, strbuf_##name, iniStringBufSize, ini_path.c_str()); \
    Config::##name = std::string(strbuf_##name)

void Config::loadConfig() {
    auto ini_path = Config::base_directory + "\\Retail\\ZHM5Randomizer.ini";

    LOAD_INI_STRING_ENTRY(worldInventoryRandomizer, iniMainCategory, "DEFAULT");
    LOAD_INI_STRING_ENTRY(heroInventoryRandomizer, iniMainCategory, "DEFAULT");
    LOAD_INI_STRING_ENTRY(npcInventoryRandomizer, iniMainCategory, "DEFAULT");
    LOAD_INI_STRING_ENTRY(stashInventoryRandomizer, iniMainCategory, "DEFAULT");
    LOAD_INI_ENTRY(randomizeNPCGrenades, iniMainCategory, 0);
    LOAD_INI_ENTRY(RNGSeed, iniMainCategory, 0);

    LOAD_INI_ENTRY(showDebugConsole, "Debug", 0);
    LOAD_INI_ENTRY(enableDebugLogging, "Debug", 0);
    LOAD_INI_ENTRY(logToFile, "Debug", 0);
}