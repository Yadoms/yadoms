#pragma once
#include <shared/plugin/yPluginApi/historization/SingleHistorizableData.hpp>
#include <shared/enumeration/EnumHelpers.hpp>


namespace specificHistorizers
{
	DECLARE_ENUM_HEADER(ECustomizeIconType,
	                    ((Battery))
	                    ((Email))
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
