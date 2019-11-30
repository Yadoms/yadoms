#pragma once
#include <shared/enumeration/EnumHelpers.hpp>
#include <shared/plugin/yPluginApi/historization/SingleHistorizableData.hpp>

namespace specificHistorizers
{
	//-----------------------------------------------------
	///\brief  All periods
	//-----------------------------------------------------   

	DECLARE_ENUM_HEADER(EWESPeriodHCWeekDayofweek,
	((LowCostWeek))
		((NormalCostWeek))
		((LowCostWeekEnd))
		((NormalCostWeekEnd))
		((LowCostDayOfWeek))
		((NormalCostDayOfWeek))
		((NotDefined)(6))
		((FrameError)(7))
		((SignalLoss)(8))
		);

   //-----------------------------------------------------
   ///\brief A Running Period object
   //-----------------------------------------------------
   class CPeriodHCWeekDayofweek : public shared::plugin::yPluginApi::historization::CSingleHistorizableData<EWESPeriodHCWeekDayofweek>
   {
   public:
      //-----------------------------------------------------
      ///\brief                     Constructor
      ///\param[in] keywordName     Yadoms keyword name
      //-----------------------------------------------------
      explicit CPeriodHCWeekDayofweek(const std::string& keywordName);

      //-----------------------------------------------------
      ///\brief                     Destructor
      //-----------------------------------------------------
      virtual ~CPeriodHCWeekDayofweek();
   };
} // namespace specificHistorizers