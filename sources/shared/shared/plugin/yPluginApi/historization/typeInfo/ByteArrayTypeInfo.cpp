#include "stdafx.h"
#include "ByteArrayTypeInfo.h"

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
               CByteArrayTypeInfo CByteArrayTypeInfo::Empty;

               CByteArrayTypeInfo::CByteArrayTypeInfo()
               {
               }

               CByteArrayTypeInfo::~CByteArrayTypeInfo()
               {
               }

               CByteArrayTypeInfo& CByteArrayTypeInfo::setMaxLength(const int maxLength)
               {
                  m_maxLength = maxLength;
                  return *this;                  
               }
               
               CDataContainer CByteArrayTypeInfo::serialize() const
               {
                  CDataContainer serializedData;
                  if (m_maxLength.isDefined())
                     serializedData.set("maxLength", m_maxLength());
                  return serializedData;
               }
            }
         }
      }
   }
} // namespace shared::plugin::yPluginApi::historization::typeInfo


