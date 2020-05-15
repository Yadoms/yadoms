#pragma once
#include <shared/plugin/yPluginApi/historization/SingleHistorizableData.hpp>
#include <shared/enumeration/EnumHelpers.hpp>


namespace specificHistorizers
{
	DECLARE_ENUM_HEADER(ECustomizeIconType,
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

	class CCustomizeIconType : public shared::plugin::yPluginApi::historization::CSingleHistorizableData<
			ECustomizeIconType>
	{
	public:
		CCustomizeIconType(const std::string& keywordName);
		virtual ~CCustomizeIconType();
	};
} // namespace specificHistorizers
