#pragma once
#include <shared/enumeration/EnumHelpers.hpp>
#include <shared/plugin/yPluginApi/historization/SingleHistorizableData.hpp>

//TODO V2 : To be copy in a library teleInfo
namespace teleInfo { namespace specificHistorizers

{   
   //-----------------------------------------------------
   ///\brief  All periods
   //-----------------------------------------------------   
   DECLARE_ENUM_HEADER(EPeriod,
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
   );

   //-----------------------------------------------------
   ///\brief A Running Period object
   //-----------------------------------------------------
   class CPeriod : public shared::plugin::yPluginApi::historization::CSingleHistorizableData<EPeriod>
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

} } // namespace teleInfo::specificHistorizer