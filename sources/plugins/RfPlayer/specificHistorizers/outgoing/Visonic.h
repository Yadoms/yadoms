#pragma once

#include <shared/plugin/yPluginApi/IYPluginApi.h>
#include <shared/enumeration/EnumHelpers.hpp>
#include <shared/plugin/yPluginApi/historization/SingleHistorizableData.hpp>

namespace yApi = shared::plugin::yPluginApi;

namespace specificHistorizers {
namespace outgoing {
     
   DECLARE_ENUM_HEADER(EVisonic,
      ((Off)(0))
      ((On)(1))
      ((Dim)(2))
      ((Assoc)(6))
   )

   class CVisonicKeyword : public shared::plugin::yPluginApi::historization::CSingleHistorizableData<EVisonic>
   {
   public:
      //-----------------------------------------------------
      ///\brief                     Constructor
      ///\param[in] keywordName     Yadoms keyword name
      //-----------------------------------------------------
      explicit CVisonicKeyword(const std::string& keywordName);

      //-----------------------------------------------------
      ///\brief                     Destructor
      //-----------------------------------------------------
      virtual ~CVisonicKeyword();
   };


} //namespace outgoin
} //namespace specificHistorizers