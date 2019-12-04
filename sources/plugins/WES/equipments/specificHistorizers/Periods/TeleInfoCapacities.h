#pragma once
#include <shared/plugin/yPluginApi/StandardCapacity.h>

namespace specificHistorizers
{
   //-----------------------------------------------------
   ///\brief A Running Period object
   //-----------------------------------------------------

	class CTeleInfoCapacities
	{
	public:
		static const shared::plugin::yPluginApi::CStandardCapacity& PeriodCapacity();
	};
} // namespace specificHistorizers