#include <Windows.h>
#include <sstream>
#include <iomanip>
#include <fstream>
#include "Console.h"
#include "Config.h"

void Console::spawn()
{
	AllocConsole();
	FILE* stream;
	freopen_s(&stream, "CONOUT$", "w", stdout);
}

//TODO: Implement file logging properly
void Console::log(const char* fmt, ...) {
	if (!Config::enableDebugLogging)
		return;

	va_list ap;
	va_start(ap, fmt);
	vprintf(fmt, ap);

	//if (Config::logToFile) {
	//	char buf[256];
	//	vsprintf_s(buf, fmt, ap);

	//	auto log_path = Config::base_directory + "/Retail/ZHM5Randomizer.log";
	//	std::ofstream ofs(log_path.c_str(), std::ofstream::out || std::ofstream::app );
	//	if (!ofs.is_open())
	//		printf("Failed to open log file: %s\n", log_path.c_str());
	//	ofs.write(buf, strlen(buf));
	//	ofs.close();

	//	printf(buf);
	//}
	//else {
	//	vprintf(fmt, ap);
	//}
}
