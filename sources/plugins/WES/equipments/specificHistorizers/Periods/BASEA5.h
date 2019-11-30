#pragma once
#include <shared/enumeration/EnumHelpers.hpp>
#include <shared/plugin/yPluginApi/historization/SingleHistorizableData.hpp>

namespace specificHistorizers
{
	//-----------------------------------------------------
	///\brief  All periods
	//-----------------------------------------------------   

	DECLARE_ENUM_HEADER(EWESPeriodBASEA5,
	    ((WinterNormalCost))
		((WinterLowCost))
		((SummerNormalCost))
		((SummerLowCost))
		((PeakCostHours))
		((NotDefined)(6))
		((FrameError)(7))
		((SignalLoss)(8))
		);

   //-----------------------------------------------------
   ///\brief A Running Period object
   //-----------------------------------------------------
   class CPeriodBaseA5 : public shared::plugin::yPluginApi::historization::CSingleHistorizableData<EWESPeriodBASEA5>
   {
   public:
      //-----------------------------------------------------
      ///\brief                     Constructor
      ///\param[in] keywordName     Yadoms keyword name
      //-----------------------------------------------------
      explicit CPeriodBaseA5(const std::string& keywordName);

      //-----------------------------------------------------
      ///\brief                     Destructor
      //-----------------------------------------------------
      virtual ~CPeriodBaseA5();
   };
} // namespace specificHistorizers