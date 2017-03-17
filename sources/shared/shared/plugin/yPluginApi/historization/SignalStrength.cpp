#include "stdafx.h"
#include "SignalStrength.h"
#include "../StandardCapacities.h"


namespace shared
{
   namespace plugin
   {
      namespace yPluginApi
      {
         namespace historization
         {
            CSignalStrength::CSignalStrength(const std::string& keywordName,
                         const EMeasureType& measureType,
                         typeInfo::CIntTypeInfo& additionalInfo)
               : CSingleHistorizableData<int>(keywordName,
                                              CStandardCapacities::SignalStrength(),
                                              EKeywordAccessMode::kGet,
                                              0,
                                              measureType,
                                              additionalInfo)
            {
            }

            CSignalStrength::~CSignalStrength()
            {
            }


            int CSignalStrength::Normalize(int signalStength)
            {
               if (signalStength > 100)
                  return 100;
               if (signalStength < 0)
                  return 0;
               return signalStength;
            }
         }
      }
   }
} // namespace shared::plugin::yPluginApi::historization


