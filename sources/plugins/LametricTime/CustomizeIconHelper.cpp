#include "CustomizeIconHelper.h"

const std::string CCustomizeIconHelper::YadomsIcon("i31581");
const std::string CCustomizeIconHelper::EmailIcon("43");
const std::string CCustomizeIconHelper::Battery100Icon("6358");


std::string CCustomizeIconHelper::getIconType(std::string& iconType)
{

	if(iconType == specificHistorizers::ECustomizeIconType::kBattery.toString())
	{
		return Battery100Icon;
	}
	if(iconType == specificHistorizers::ECustomizeIconType::kEmail.toString())
	{
		return EmailIcon;
	}
	return YadomsIcon;
}
