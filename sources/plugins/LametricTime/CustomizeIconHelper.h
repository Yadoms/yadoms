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

};
