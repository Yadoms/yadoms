#pragma once

#include <shared/plugin/yPluginApi/IYPluginApi.h>
#include <shared/enumeration/EnumHelpers.hpp>
#include <shared/plugin/yPluginApi/historization/SingleHistorizableData.hpp>

namespace yApi = shared::plugin::yPluginApi;

namespace specificHistorizers {
namespace incoming {
      
   DECLARE_ENUM_HEADER(EType1StateValues,
      ((Off)(0))
      ((On)(1))
      ((AllOff)(4))
      ((AllOn)(5))
   )

   class CType1State : public yApi::historization::CSingleHistorizableData<EType1StateValues>
   {
   public:
      //-----------------------------------------------------
      ///\brief                     Constructor
      ///\param[in] keywordName     Yadoms keyword name
      //-----------------------------------------------------
      explicit CType1State(const std::string& keywordName);

      //-----------------------------------------------------
      ///\brief                     Destructor
      //-----------------------------------------------------
      virtual ~CType1State();
   };

} //namespace incoming
} //namespace specificHistorizers