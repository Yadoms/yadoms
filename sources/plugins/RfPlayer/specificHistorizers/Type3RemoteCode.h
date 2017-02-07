#pragma once

#include <shared/plugin/yPluginApi/IYPluginApi.h>
#include <shared/enumeration/EnumHelpers.hpp>
#include <shared/plugin/yPluginApi/historization/SingleHistorizableData.hpp>

namespace yApi = shared::plugin::yPluginApi;

namespace specificHistorizers {
      
   DECLARE_ENUM_HEADER(EType3RemoteCodeValues,
      ((Left)(5))
      ((Right)(6))
   )

   class CType3RemoteCode : public yApi::historization::CSingleHistorizableData<EType3RemoteCodeValues>
   {
   public:
      //-----------------------------------------------------
      ///\brief                     Constructor
      ///\param[in] keywordName     Yadoms keyword name
      //-----------------------------------------------------
      explicit CType3RemoteCode(const std::string& keywordName);

      //-----------------------------------------------------
      ///\brief                     Destructor
      //-----------------------------------------------------
      virtual ~CType3RemoteCode();
   };

} //namespace specificHistorizers