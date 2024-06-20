#pragma once
#include <string>
#include <vector>
#include <boost/date_time.hpp>

namespace KrilloudDebug
{
	void SetDebugFilename(std::string filename);
	void ActivateDebug(bool active);
	bool GetDebugStatus();
	const std::string currentDateTime();
	void DebugWrite(std::string debugText);
	void DebugWrite(std::vector<std::string> debugText);
	void DumpDebug();
}