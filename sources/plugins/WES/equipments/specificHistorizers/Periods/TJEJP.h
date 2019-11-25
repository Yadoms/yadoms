#pragma once
#include <shared/Export.h>
#include <shared/enumeration/EnumHelpers.hpp>
#include <shared/plugin/yPluginApi/historization/SingleHistorizableData.hpp>

namespace specificHistorizers
{
	//-----------------------------------------------------
	///\brief  All periods
	//-----------------------------------------------------   

	DECLARE_ENUM_HEADER(EWESPeriodTJEJP,
	((MobilePeakCostHours))
		((WinterCost))
		((SummerNormalCost))
		((SummerLowCost))
		((NotDefined))
		);

   //-----------------------------------------------------
   ///\brief A Running Period object
   //-----------------------------------------------------
   class CPeriodTJEJP : public shared::plugin::yPluginApi::historization::CSingleHistorizableData<EWESPeriodTJEJP>
   {
   public:
      //-----------------------------------------------------
      ///\brief                     Constructor
      ///\param[in] keywordName     Yadoms keyword name
      //-----------------------------------------------------
      explicit CPeriodTJEJP(const std::string& keywordName);

      //-----------------------------------------------------
      ///\brief                     Destructor
      //-----------------------------------------------------
      virtual ~CPeriodTJEJP();
   };
} // namespace specificHistorizers