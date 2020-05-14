#include "stdafx.h"
#include "CustomizeIconType.h"
#include <shared/plugin/yPluginApi/StandardUnits.h>

namespace specificHistorizers
{
	DECLARE_ENUM_IMPLEMENTATION(ECustomizeIconType,
	                            ((Battery))
	                            ((Email))

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
