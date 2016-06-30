#include "stdafx.h"
#include "StringTypeInfo.h"

namespace shared
{
   namespace plugin
   {
      namespace yPluginApi
      {
         namespace historization
         {
            namespace typeInfo
            {
               CStringTypeInfo CStringTypeInfo::Empty;

               CStringTypeInfo::CStringTypeInfo()
               {
               }

               CStringTypeInfo::~CStringTypeInfo()
               {
               }

               CStringTypeInfo& CStringTypeInfo::setRegexString(const std::string& regexString)
               {
                  m_regex = regexString;
                  return *this;
               }

               CDataContainer CStringTypeInfo::serialize() const
               {
                  CDataContainer serializedData;
                  if (m_regex.isDefined())
                     serializedData.set("regex", m_regex());
                  return serializedData;
               }
            }
         }
      }
   }
} // namespace shared::plugin::yPluginApi::historization::typeInfo


