#include "stdafx.h"
#include "CustomizeIconType.h"
#include <shared/plugin/yPluginApi/StandardUnits.h>

namespace specificHistorizers
{
	DECLARE_ENUM_IMPLEMENTATION(ECustomizeIconType,
	                            ((Email))
	                            ((Battery100))
	                            ((Battery80))
	                            ((Battery60))
	                            ((Battery40))
	                            ((Battery20))
	                            ((BatteryLowAnimated))
	                            ((ThermometerLow))
	                            ((ThermometerOutdoor))
	                            ((ThermometerAnimated))
	                            ((WaterDrop))
	                            ((WaterDropAnimated))
	                            ((Sun))
	                            ((CloudySunAnimated))
	                            ((None))

	);

	DECLARE_CAPACITY(CustomizeIconTypeCapacity,
	                 "CustomizeIconType",
	                 shared::plugin::yPluginApi::CStandardUnits::NoUnit(),
	                 shared::plugin::yPluginApi::EKeywordDataType::kEnum);

	CCustomizeIconType::CCustomizeIconType(const std::string& keywordName)
		: CSingleHistorizableData<ECustomizeIconType>(keywordName,
		                                              CustomizeIconTypeCapacity(),
		                                              shared::plugin::yPluginApi::EKeywordAccessMode::kGetSet)
	{
	}

	CCustomizeIconType::~CCustomizeIconType()
	{
	}
} // namespace specificHistorizers
