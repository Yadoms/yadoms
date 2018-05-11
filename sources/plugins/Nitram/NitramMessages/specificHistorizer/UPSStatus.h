#pragma once
#include <shared/Export.h>
#include <shared/enumeration/EnumHelpers.hpp>
#include <shared/plugin/yPluginApi/historization/SingleHistorizableData.hpp>

namespace nitram { namespace specificHistorizers

{   
   //-----------------------------------------------------
   ///\brief  All status of the UPS
   //-----------------------------------------------------   
   DECLARE_ENUM_HEADER(EUPSState,
   ((Charging))
      ((Discharging))
      ((FullyCharged))
      );

   //-----------------------------------------------------
   ///\brief A Running Period object
   //-----------------------------------------------------
   class CStatus : public shared::plugin::yPluginApi::historization::CSingleHistorizableData<EUPSState>
   {
   public:
      //-----------------------------------------------------
      ///\brief                     Constructor
      ///\param[in] keywordName     Yadoms keyword name
      //-----------------------------------------------------
      explicit CStatus(const std::string& keywordName);

      //-----------------------------------------------------
      ///\brief                     Destructor
      //-----------------------------------------------------
      virtual ~CStatus();
   };

} } // namespace nitram::specificHistorizer