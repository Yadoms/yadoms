#pragma once
#include <shared/enumeration/EnumHelpers.hpp>
#include <shared/plugin/yPluginApi/historization/SingleHistorizableData.hpp>

namespace specificHistorizers
{
	//-----------------------------------------------------
	///\brief  All periods
	//-----------------------------------------------------   

	DECLARE_ENUM_HEADER(EWESPeriodHTA5,
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
   class CPeriodHTA5 : public shared::plugin::yPluginApi::historization::CSingleHistorizableData<EWESPeriodHTA5>
   {
   public:
      //-----------------------------------------------------
      ///\brief                     Constructor
      ///\param[in] keywordName     Yadoms keyword name
      //-----------------------------------------------------
      explicit CPeriodHTA5(const std::string& keywordName);

      //-----------------------------------------------------
      ///\brief                     Destructor
      //-----------------------------------------------------
      virtual ~CPeriodHTA5();
   };
} // namespace specificHistorizers