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
            ///\brief A Alarm historizable object
            //-----------------------------------------------------
            class YADOMS_SHARED_EXPORT CAlarm : public CSingleHistorizableData<bool>
            {
            public:
               //-----------------------------------------------------
               ///\brief                     Constructor
               ///\param[in] keywordName     Yadoms keyword name
               ///\param[in] accessMode      The access mode
               //-----------------------------------------------------
               explicit CAlarm(const std::string& keywordName, const EKeywordAccessMode& accessMode = EKeywordAccessMode::kGetSet, typeInfo::CBoolTypeInfo &ti = typeInfo::CBoolTypeInfo::Empty);

               //-----------------------------------------------------
               ///\brief                     Destructor
               //-----------------------------------------------------
               virtual ~CAlarm();
            };
         }
      }
   }
} // namespace shared::plugin::yPluginApi::historization


