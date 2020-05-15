#pragma once
#include <string>
#include "CustomizeIconType.h"

class CCustomizeIconHelper
{
public:
	static std::string getIconType(std::string& iconType);
	
	const static std::string YadomsIcon;
	const static std::string EmailIcon;
	const static std::string Battery100Icon;
	const static std::string Battery80Icon;
	const static std::string Battery60Icon;
	const static std::string Battery40Icon;
	const static std::string Battery20Icon;
	const static std::string BatteryLowAnimatedIcon;
	const static std::string ThermometerLowIcon;
	const static std::string ThermometerOutdoorIcon;
	const static std::string ThermometerAnimatedIcon;
	const static std::string WaterDropIcon;
	const static std::string WaterDropAnimatedIcon;
	const static std::string SunIcon;
	const static std::string CloudySunAnimatedIcon;
};
