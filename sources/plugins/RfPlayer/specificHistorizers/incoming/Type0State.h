#pragma once

#include <shared/plugin/yPluginApi/IYPluginApi.h>
#include <shared/enumeration/EnumHelpers.hpp>
#include <shared/plugin/yPluginApi/historization/SingleHistorizableData.hpp>

namespace yApi = shared::plugin::yPluginApi;

namespace specificHistorizers {
namespace incoming {
      
   DECLARE_ENUM_HEADER(EType0StateValues,
      ((Off))
      ((On))
      ((Bright))
      ((Dim))
      ((AllOff))
      ((AllOn))
   )

   class CType0State : public yApi::historization::CSingleHistorizableData<EType0StateValues>
   {
   public:
      //-----------------------------------------------------
      ///\brief                     Constructor
      ///\param[in] keywordName     Yadoms keyword name
      //-----------------------------------------------------
      explicit CType0State(const std::string& keywordName);

      //-----------------------------------------------------
      ///\brief                     Destructor
      //-----------------------------------------------------
      virtual ~CType0State();
   };

} //namespace incoming
} //namespace specificHistorizers