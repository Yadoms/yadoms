#pragma once
#include <shared/plugin/yPluginApi/historization/SingleHistorizableData.hpp>
#include <shared/plugin/yPluginApi/typeInfo/IntTypeInfo.h>

namespace shared
{
   namespace plugin
   {
      namespace yPluginApi
      {
         namespace historization
         {
            //-----------------------------------------------------
            ///\brief A temperature historizable object
            //-----------------------------------------------------
            class CKByte : public CSingleHistorizableData<long>
            {
            public:
               //-----------------------------------------------------
               ///\brief                     Constructor
               ///\param[in] keywordName     Yadoms keyword name
               ///\param[in] accessMode      The access mode
               ///\param[in] measureType     To be used as increment counter (values will be added to current database value) or totalizer
               //-----------------------------------------------------
               explicit CKByte(const std::string& keywordName,
                               const EKeywordAccessMode& accessMode = EKeywordAccessMode::kGet,
                               const EMeasureType& measureType = EMeasureType::kAbsolute,
                               const typeInfo::CIntTypeInfo& additionalInfo = typeInfo::CIntTypeInfo::Empty);

               virtual ~CKByte() = default;
            };
         }
      }
   }
} // namespace shared::plugin::yPluginApi::historization


