#pragma once

#include <shared/plugin/yPluginApi/IYPluginApi.h>
#include <shared/enumeration/EnumHelpers.hpp>
#include <shared/plugin/yPluginApi/historization/SingleHistorizableData.hpp>

namespace yApi = shared::plugin::yPluginApi;

namespace specificHistorizers {
namespace outgoing {
     
   DECLARE_ENUM_HEADER(EDomia,
      ((Off)(0))
      ((On)(1))
      ((Dim)(2))
      ((Assoc)(6))
   )

   class CDomiaKeyword : public shared::plugin::yPluginApi::historization::CSingleHistorizableData<EDomia>
   {
   public:
      //-----------------------------------------------------
      ///\brief                     Constructor
      ///\param[in] keywordName     Yadoms keyword name
      //-----------------------------------------------------
      explicit CDomiaKeyword(const std::string& keywordName);

      //-----------------------------------------------------
      ///\brief                     Destructor
      //-----------------------------------------------------
      virtual ~CDomiaKeyword();
   };


} //namespace outgoin
} //namespace specificHistorizers