#pragma once
#include <shared/Export.h>
#include <shared/enumeration/EnumHelpers.hpp>
#include <shared/plugin/yPluginApi/historization/SingleHistorizableData.hpp>

namespace specificHistorizers
{
	//-----------------------------------------------------
	///\brief  All periods
	//-----------------------------------------------------   

	DECLARE_ENUM_HEADER(EWESPeriodBT4SUP36,
	((WinterNormalCost))
		((WinterLowCost))
		((SummerNormalCost))
		((SummerLowCost))
		((NotDefined))
		);

   //-----------------------------------------------------
   ///\brief A Running Period object
   //-----------------------------------------------------
   class CPeriodBT4SUP36 : public shared::plugin::yPluginApi::historization::CSingleHistorizableData<EWESPeriodBT4SUP36>
   {
   public:
      //-----------------------------------------------------
      ///\brief                     Constructor
      ///\param[in] keywordName     Yadoms keyword name
      //-----------------------------------------------------
      explicit CPeriodBT4SUP36(const std::string& keywordName);

      //-----------------------------------------------------
      ///\brief                     Destructor
      //-----------------------------------------------------
      virtual ~CPeriodBT4SUP36();
   };
} // namespace specificHistorizers