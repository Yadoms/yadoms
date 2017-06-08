#pragma once

#include <shared/plugin/yPluginApi/IYPluginApi.h>
#include <shared/enumeration/EnumHelpers.hpp>
#include <shared/plugin/yPluginApi/historization/SingleHistorizableData.hpp>

namespace yApi = shared::plugin::yPluginApi;

namespace specificHistorizers {
namespace incoming {
      
   DECLARE_ENUM_HEADER(EType10OperatingModeValues,
      ((Eco))
      ((Moderate))
      ((Medio))
      ((Comfort))
      ((Stop))
      ((OutOfFrost))
      ((Special))
      ((Auto))
      ((Centralised))
   )

   class CType10OperatingMode : public yApi::historization::CSingleHistorizableData<EType10OperatingModeValues>
   {
   public:
      //-----------------------------------------------------
      ///\brief                     Constructor
      ///\param[in] keywordName     Yadoms keyword name
      //-----------------------------------------------------
      explicit CType10OperatingMode(const std::string& keywordName);

      //-----------------------------------------------------
      ///\brief                     Destructor
      //-----------------------------------------------------
      virtual ~CType10OperatingMode();
   };

} //namespace incoming
} //namespace specificHistorizers