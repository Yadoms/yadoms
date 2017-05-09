#pragma once

#include <shared/plugin/yPluginApi/IYPluginApi.h>
#include <shared/enumeration/EnumHelpers.hpp>
#include <shared/plugin/yPluginApi/historization/SingleHistorizableData.hpp>

namespace yApi = shared::plugin::yPluginApi;

namespace specificHistorizers {
namespace outgoing {
     
   DECLARE_ENUM_HEADER(EBlyss,
      ((Off)(0))
      ((On)(1))
      ((Dim)(2))
      ((AllOff)(4))
      ((AllOn)(5))
      ((Assoc)(6))
   )

   class CBlyssKeyword : public shared::plugin::yPluginApi::historization::CSingleHistorizableData<EBlyss>
   {
   public:
      //-----------------------------------------------------
      ///\brief                     Constructor
      ///\param[in] keywordName     Yadoms keyword name
      //-----------------------------------------------------
      explicit CBlyssKeyword(const std::string& keywordName);

      //-----------------------------------------------------
      ///\brief                     Destructor
      //-----------------------------------------------------
      virtual ~CBlyssKeyword();
   };


} //namespace outgoin
} //namespace specificHistorizers