#include "stdafx.h"
#include "PercentageTypeInfo.h"

namespace shared
{
   namespace plugin
   {
      namespace yPluginApi
      {
         namespace typeInfo
         {
            CPercentageTypeInfo::CPercentageTypeInfo()
               : m_intTypeInfo(0, 100, 1)
            {
            }

            boost::shared_ptr<CDataContainer> CPercentageTypeInfo::serialize() const
            {
               return m_intTypeInfo.serialize();
            }
         }
      }
   }
} // namespace shared::plugin::yPluginApi::historization::typeInfo
