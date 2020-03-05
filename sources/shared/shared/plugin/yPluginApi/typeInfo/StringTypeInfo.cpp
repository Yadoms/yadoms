#include "stdafx.h"
#include "StringTypeInfo.h"

namespace shared
{
   namespace plugin
   {
      namespace yPluginApi
      {
         namespace typeInfo
         {
            const CStringTypeInfo CStringTypeInfo::Empty;

            CStringTypeInfo& CStringTypeInfo::setRegexString(const std::string& regexString)
            {
               m_regex = regexString;
               return *this;
            }

            CDataContainerSharedPtr CStringTypeInfo::serialize() const
            {
               CDataContainerSharedPtr serializedData = new_CDataContainerSharedPtr();
               if (m_regex.isDefined())
                  serializedData->set("regex", m_regex());
               return serializedData;
            }
         }
      }
   }
} // namespace shared::plugin::yPluginApi::historization::typeInfo
