#pragma once
#include <shared/enumeration/EnumHelpers.hpp>
#include <shared/plugin/yPluginApi/historization/SingleHistorizableData.hpp>

namespace specificHistorizers
{
	//-----------------------------------------------------
	///\brief  All periods
	//-----------------------------------------------------   

	DECLARE_ENUM_HEADER(EWESPeriodTempo,
		((LowCostBlueDays))
		((LowCostWhiteDays))
		((LowCostRedDays))
		((NormalCostBlueDays))
		((NormalCostWhiteDays))
		((NormalCostRedDays))
		((NotDefined))
	);

   //-----------------------------------------------------
   ///\brief A Running Period object
   //-----------------------------------------------------
   class CPeriodTempo : public shared::plugin::yPluginApi::historization::CSingleHistorizableData<EWESPeriodTempo>
   {
   public:
      //-----------------------------------------------------
      ///\brief                     Constructor
      ///\param[in] keywordName     Yadoms keyword name
      //-----------------------------------------------------
      explicit CPeriodTempo(const std::string& keywordName);

      //-----------------------------------------------------
      ///\brief                     Destructor
      //-----------------------------------------------------
      virtual ~CPeriodTempo();
   };
} // namespace specificHistorizers