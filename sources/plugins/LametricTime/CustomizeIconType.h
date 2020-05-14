#pragma once
#include <shared/plugin/yPluginApi/historization/SingleHistorizableData.hpp>
#include <shared/enumeration/EnumHelpers.hpp>


namespace specificHistorizers
{
	DECLARE_ENUM_HEADER(ECustomizeIconType,
	                    ((Battery))
	);

	class CCustomizeIconType : public shared::plugin::yPluginApi::historization::CSingleHistorizableData<ECustomizeIconType>
	{
		CCustomizeIconType(const std::string& keywordName);
		virtual ~CCustomizeIconType();
	};
}// namespace specificHistorizers
