#pragma once
#include <shared/Export.h>
#include <shared/plugin/yPluginApi/historization/SingleHistorizableData.hpp>
#include "typeInfo/BoolTypeInfo.h"

namespace shared
{
   namespace plugin
   {
      namespace yPluginApi
      {
         namespace historization
         {
            //-----------------------------------------------------
            ///\brief A switch historizable object
            //-----------------------------------------------------
            class YADOMS_SHARED_EXPORT CSwitch : public CSingleHistorizableData<bool>
            {
            public:
               //-----------------------------------------------------
               ///\brief                     Constructor
               ///\param[in] keywordName     Yadoms keyword name
               ///\param[in] accessMode      The access mode
               //-----------------------------------------------------
               explicit CSwitch(const std::string& keywordName,
                                const EKeywordAccessMode& accessMode = EKeywordAccessMode::kGetSet,
                                typeInfo::CBoolTypeInfo& ti = typeInfo::CBoolTypeInfo::Empty);

               //-----------------------------------------------------
               ///\brief                     Destructor
               //-----------------------------------------------------
               virtual ~CSwitch();
            };
         }
      }
   }
} // namespace shared::plugin::yPluginApi::historization


