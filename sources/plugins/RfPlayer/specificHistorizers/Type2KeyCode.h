#pragma once

#include <shared/plugin/yPluginApi/IYPluginApi.h>
#include <shared/enumeration/EnumHelpers.hpp>
#include <shared/plugin/yPluginApi/historization/SingleHistorizableData.hpp>

namespace yApi = shared::plugin::yPluginApi;

namespace specificHistorizers {
      
   DECLARE_ENUM_HEADER(EType2KeyCodeValues,
      ((Button1)(0x08))
      ((Button2)(0x10))
      ((Button3)(0x20))
      ((Button4)(0x40))
   )

   class CType2KeyCode : public yApi::historization::CSingleHistorizableData<EType2KeyCodeValues>
   {
   public:
      //-----------------------------------------------------
      ///\brief                     Constructor
      ///\param[in] keywordName     Yadoms keyword name
      //-----------------------------------------------------
      explicit CType2KeyCode(const std::string& keywordName);

      //-----------------------------------------------------
      ///\brief                     Destructor
      //-----------------------------------------------------
      virtual ~CType2KeyCode();
   };

} //namespace specificHistorizers