#pragma once
#include <shared/enumeration/EnumHelpers.hpp>
#include <shared/plugin/yPluginApi/historization/SingleHistorizableData.hpp>

namespace specificHistorizers
{
	//-----------------------------------------------------
	///\brief  All periods
	//-----------------------------------------------------   

	DECLARE_ENUM_HEADER(EWESPeriodHCWE,
       ((LowCostHours))
       ((NormalCostHours))
       ((WEHours))
	   ((NotDefined)(6))
    );

   //-----------------------------------------------------
   ///\brief A Running Period object
   //-----------------------------------------------------
   class CPeriodHCWE : public shared::plugin::yPluginApi::historization::CSingleHistorizableData<EWESPeriodHCWE>
   {
   public:
      //-----------------------------------------------------
      ///\brief                     Constructor
      ///\param[in] keywordName     Yadoms keyword name
      //-----------------------------------------------------
      explicit CPeriodHCWE(const std::string& keywordName);

      //-----------------------------------------------------
      ///\brief                     Destructor
      //-----------------------------------------------------
      virtual ~CPeriodHCWE();
   };
} // namespace specificHistorizers