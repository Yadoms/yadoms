#include "CustomizeIconHelper.h"

const std::string CCustomizeIconHelper::YadomsIcon("i31581");
const std::string CCustomizeIconHelper::EmailIcon("43");
const std::string CCustomizeIconHelper::Battery100Icon("6358");
const std::string CCustomizeIconHelper::Battery80Icon("6357");
const std::string CCustomizeIconHelper::Battery60Icon("6356");
const std::string CCustomizeIconHelper::Battery40Icon("6355");
const std::string CCustomizeIconHelper::Battery20Icon("6354");
const std::string CCustomizeIconHelper::BatteryLowAnimatedIcon("12123");
const std::string CCustomizeIconHelper::ThermometerLowIcon("37833");
const std::string CCustomizeIconHelper::ThermometerOutdoorIcon("21180");
const std::string CCustomizeIconHelper::ThermometerAnimatedIcon("12144");
const std::string CCustomizeIconHelper::WaterDropIcon("8990");
const std::string CCustomizeIconHelper::WaterDropAnimatedIcon("12989");
const std::string CCustomizeIconHelper::SunIcon("13189");
const std::string CCustomizeIconHelper::CloudySunAnimatedIcon("8756");

std::string CCustomizeIconHelper::getIconType(std::string& iconType)
{
	if (iconType == specificHistorizers::ECustomizeIconType::kYadoms.toString())
	{
		return YadomsIcon;
	}
	if (iconType == specificHistorizers::ECustomizeIconType::kEmail.toString())
	{
		return EmailIcon;
	}

	if (iconType == specificHistorizers::ECustomizeIconType::kBattery100.toString())
	{
		return Battery100Icon;
	}

	if (iconType == specificHistorizers::ECustomizeIconType::kBattery80.toString())
	{
		return Battery80Icon;
	}

	if (iconType == specificHistorizers::ECustomizeIconType::kBattery60.toString())
	{
		return Battery60Icon;
	}

	if (iconType == specificHistorizers::ECustomizeIconType::kBattery40.toString())
	{
		return Battery40Icon;
	}

	if (iconType == specificHistorizers::ECustomizeIconType::kBattery20.toString())
	{
		return Battery20Icon;
	}

	if (iconType == specificHistorizers::ECustomizeIconType::kBatteryLowAnimated.toString())
	{
		return BatteryLowAnimatedIcon;
	}

	if (iconType == specificHistorizers::ECustomizeIconType::kThermometerLow.toString())
	{
		return ThermometerLowIcon;
	}

	if (iconType == specificHistorizers::ECustomizeIconType::kThermometerOutdoor.toString())
	{
		return ThermometerOutdoorIcon;
	}

	if (iconType == specificHistorizers::ECustomizeIconType::kThermometerAnimated.toString())
	{
		return ThermometerAnimatedIcon;
	}

	if (iconType == specificHistorizers::ECustomizeIconType::kWaterDrop.toString())
	{
		return WaterDropIcon;
	}

	if (iconType == specificHistorizers::ECustomizeIconType::kWaterDropAnimated.toString())
	{
		return WaterDropAnimatedIcon;
	}

	if (iconType == specificHistorizers::ECustomizeIconType::kSun.toString())
	{
		return SunIcon;
	}

	if (iconType == specificHistorizers::ECustomizeIconType::kCloudySunAnimated.toString())
	{
		return CloudySunAnimatedIcon;
	}

	return YadomsIcon;
}
