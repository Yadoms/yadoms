#pragma once
#include <shared/Export.h>
#include "SingleHistorizableData.hpp"
#include "typeInfo/StringTypeInfo.h"

namespace shared
{
   namespace plugin
   {
      namespace yPluginApi
      {
         namespace historization
         {
            //-----------------------------------------------------
            ///\brief A UserCode historizable object
            //-----------------------------------------------------
            class YADOMS_SHARED_EXPORT CUserCode : public CSingleHistorizableData<std::string>
            {
            public:
               //-----------------------------------------------------
               ///\brief                     Constructor
               ///\param[in] keywordName     Yadoms keyword name
               ///\param[in] accessMode      The access mode
               ///\param[in] measureType     The measure type (normally kAbsolute)
               //-----------------------------------------------------
               explicit CUserCode(const std::string& keywordName,
                            const EKeywordAccessMode& accessMode = EKeywordAccessMode::kGet,
                            const EMeasureType& measureType = EMeasureType::kAbsolute,
                            typeInfo::CStringTypeInfo& additionalInfo = typeInfo::CStringTypeInfo::Empty);

               //-----------------------------------------------------
               ///\brief                     Destructor
               //-----------------------------------------------------
               virtual ~CUserCode();
            };
         }
      }
   }
} // namespace shared::plugin::yPluginApi::historization


