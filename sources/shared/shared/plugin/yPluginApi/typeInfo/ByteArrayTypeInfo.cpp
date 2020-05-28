#include "stdafx.h"
#include "ByteArrayTypeInfo.h"

namespace shared
{
   namespace plugin
   {
      namespace yPluginApi
      {
         namespace typeInfo
         {
            const CByteArrayTypeInfo CByteArrayTypeInfo::Empty;

            CByteArrayTypeInfo& CByteArrayTypeInfo::setMaxLength(const int maxLength)
            {
               m_maxLength = maxLength;
               return *this;
            }

            boost::shared_ptr<CDataContainer> CByteArrayTypeInfo::serialize() const
            {
               boost::shared_ptr<CDataContainer> serializedData = shared::CDataContainer::make();
               if (m_maxLength.isDefined())
                  serializedData->set("maxLength", m_maxLength());
               return serializedData;
            }
         }
      }
   }
} // namespace shared::plugin::yPluginApi::historization::typeInfo
