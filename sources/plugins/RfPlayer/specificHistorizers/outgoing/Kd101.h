#pragma once

#include <shared/plugin/yPluginApi/IYPluginApi.h>
#include <shared/enumeration/EnumHelpers.hpp>
#include <shared/plugin/yPluginApi/historization/SingleHistorizableData.hpp>

namespace yApi = shared::plugin::yPluginApi;

namespace specificHistorizers {
namespace outgoing {
     
   DECLARE_ENUM_HEADER(EKd101,
      ((Off)(0))
      ((On)(1))
      ((Dim)(2))
      ((Assoc)(6))
   )

   class CKd101Keyword : public shared::plugin::yPluginApi::historization::CSingleHistorizableData<EKd101>
   {
   public:
      //-----------------------------------------------------
      ///\brief                     Constructor
      ///\param[in] keywordName     Yadoms keyword name
      //-----------------------------------------------------
      explicit CKd101Keyword(const std::string& keywordName);

      //-----------------------------------------------------
      ///\brief                     Destructor
      //-----------------------------------------------------
      virtual ~CKd101Keyword();
   };


} //namespace outgoin
} //namespace specificHistorizers