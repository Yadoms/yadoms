#pragma once
#include <string>
#include "CustomizeIconType.h"

class CIconGenerator
{
public:
	static std::string getIconType(std::string& iconType);

private:
	const static std::string YadomsIcon;
	const static std::string EmailIcon;
	const static std::string BatteryIcon;
};
