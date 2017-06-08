#pragma once

#include <shared/plugin/yPluginApi/IYPluginApi.h>
#include <shared/enumeration/EnumHelpers.hpp>
#include <shared/plugin/yPluginApi/historization/SingleHistorizableData.hpp>

namespace yApi = shared::plugin::yPluginApi;

namespace specificHistorizers {
namespace outgoing {
     
   DECLARE_ENUM_HEADER(EChacon,
      ((Off)(0))
      ((On)(1))
      ((Dim)(2))
      ((AllOff)(4))
      ((AllOn)(5))
      ((Assoc)(6))
   )

   class CChaconKeyword : public shared::plugin::yPluginApi::historization::CSingleHistorizableData<EChacon>
   {
   public:
      //-----------------------------------------------------
      ///\brief                     Constructor
      ///\param[in] keywordName     Yadoms keyword name
      //-----------------------------------------------------
      explicit CChaconKeyword(const std::string& keywordName);

      //-----------------------------------------------------
      ///\brief                     Destructor
      //-----------------------------------------------------
      virtual ~CChaconKeyword();
   };


} //namespace outgoin
} //namespace specificHistorizers