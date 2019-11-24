#pragma once
#include <shared/Export.h>
#include <shared/enumeration/EnumHelpers.hpp>
#include <shared/plugin/yPluginApi/historization/SingleHistorizableData.hpp>

namespace specificHistorizers
{  
   //-----------------------------------------------------
   ///\brief  All periods
   //-----------------------------------------------------   
   DECLARE_ENUM_HEADER(EWESTeleInfoPeriod, // TODO : Check if possible to declare different Enum Class for a better cast into TIC class shared::enumeration::IExtendedEnum
      ((NotDefined))
      ((AllHours))
      ((LowCostHours))
      ((PeakCostHours))
      ((NormalCostHours))
      ((MobilePeakCostHours))
      ((LowCostBlueDays))
      ((LowCostWhiteDays))
      ((LowCostRedDays))
      ((NormalCostBlueDays))
      ((NormalCostWhiteDays))
      ((NormalCostRedDays))
      ((WinterNormalCost))
      ((WinterLowCost))
      ((SummerNormalCost))
      ((SummerLowCost))
      ((WinterCost))
	  ((WEHours))
      ((LowCostWeek))
	  ((NormalCostWeek))
	  ((LowCostWeekEnd))
	  ((NormalCostWeekEnd))
      ((LowCostDayOfWeek))
      ((NormalCostDayOfWeek))
   );

   //-----------------------------------------------------
   ///\brief A Running Period object
   //-----------------------------------------------------
   class CPeriod : public shared::plugin::yPluginApi::historization::CSingleHistorizableData<EWESTeleInfoPeriod>
   {
   public:
      //-----------------------------------------------------
      ///\brief                     Constructor
      ///\param[in] keywordName     Yadoms keyword name
      //-----------------------------------------------------
      explicit CPeriod(const std::string& keywordName);

      //-----------------------------------------------------
      ///\brief                     Destructor
      //-----------------------------------------------------
      virtual ~CPeriod();
   };
} // namespace specificHistorizers