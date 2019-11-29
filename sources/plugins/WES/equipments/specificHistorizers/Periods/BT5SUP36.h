#pragma once
#include <shared/enumeration/EnumHelpers.hpp>
#include <shared/plugin/yPluginApi/historization/SingleHistorizableData.hpp>

namespace specificHistorizers
{
	//-----------------------------------------------------
	///\brief  All periods
	//-----------------------------------------------------   

	DECLARE_ENUM_HEADER(EWESPeriodBT5SUP36,
	((WinterNormalCost))
		((WinterLowCost))
		((SummerNormalCost))
		((SummerLowCost))
		((PeakCostHours))
		((NotDefined))
		);

   //-----------------------------------------------------
   ///\brief A Running Period object
   //-----------------------------------------------------
   class CPeriodBT5SUP36 : public shared::plugin::yPluginApi::historization::CSingleHistorizableData<EWESPeriodBT5SUP36>
   {
   public:
      //-----------------------------------------------------
      ///\brief                     Constructor
      ///\param[in] keywordName     Yadoms keyword name
      //-----------------------------------------------------
      explicit CPeriodBT5SUP36(const std::string& keywordName);

      //-----------------------------------------------------
      ///\brief                     Destructor
      //-----------------------------------------------------
      virtual ~CPeriodBT5SUP36();
   };
} // namespace specificHistorizers