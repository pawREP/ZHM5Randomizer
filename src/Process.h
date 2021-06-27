#pragma once
#include <string>

namespace Process {

// Get filename of exe file the current process was started from.
std::wstring getModuleFilename(int moduleHandle = 0);

} // namespace Process
