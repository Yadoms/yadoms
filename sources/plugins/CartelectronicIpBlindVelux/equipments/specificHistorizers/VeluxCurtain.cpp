#include "stdafx.h"
#include "VeluxCurtain.h"
#include <shared/plugin/yPluginApi/StandardCapacities.h>

namespace specificHistorizers
{
    DECLARE_ENUM_IMPLEMENTATION(EVeluxCurtainCommand,
			((Open))
			((Stop))
            ((Close))
    );

	CVeluxCurtain::CVeluxCurtain(const std::string& keywordName)
		: shared::plugin::yPluginApi::historization::CSingleHistorizableData<EVeluxCurtainCommand>(keywordName,
			shared::plugin::yPluginApi::CStandardCapacities::Curtain(),
			shared::plugin::yPluginApi::EKeywordAccessMode::kGetSet)
    {
    }

	CVeluxCurtain::~CVeluxCurtain()
	{
	}
} // specificHistorizers
