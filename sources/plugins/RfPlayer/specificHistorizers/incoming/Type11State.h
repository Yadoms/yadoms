#pragma once

#include <shared/plugin/yPluginApi/IYPluginApi.h>
#include <shared/enumeration/EnumHelpers.hpp>
#include <shared/plugin/yPluginApi/historization/SingleHistorizableData.hpp>

namespace yApi = shared::plugin::yPluginApi;

namespace specificHistorizers {
namespace incoming {
      
   DECLARE_ENUM_HEADER(EType11StateValues,
      ((On)(1))
      ((Off)(2))
      ((Stop)(3))
   )

   class CType11State : public yApi::historization::CSingleHistorizableData<EType11StateValues>
   {
   public:
      //-----------------------------------------------------
      ///\brief                     Constructor
      ///\param[in] keywordName     Yadoms keyword name
      //-----------------------------------------------------
      explicit CType11State(const std::string& keywordName);

      //-----------------------------------------------------
      ///\brief                     Destructor
      //-----------------------------------------------------
      virtual ~CType11State();
   };

} //namespace incoming
} //namespace specificHistorizers