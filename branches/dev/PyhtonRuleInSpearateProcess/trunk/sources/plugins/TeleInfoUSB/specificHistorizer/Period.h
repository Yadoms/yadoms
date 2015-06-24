#pragma once
#include <shared/Export.h>
#include <shared/enumeration/EnumHelpers.hpp>
#include <shared/plugin/yPluginApi/historization/SingleHistorizableData.hpp>

namespace teleInfoUSB { namespace specificHistorizers
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
      ///\param[in] accessMode      The access mode
      ///\param[in] measureType     The measure type (normally kAbsolute)
      //-----------------------------------------------------
      CPeriod(const std::string& keywordName);

      //-----------------------------------------------------
      ///\brief                     Destructor
      //-----------------------------------------------------
      virtual ~CPeriod();
   };

} } // namespace teleInfoUSB::specificHistorizer

