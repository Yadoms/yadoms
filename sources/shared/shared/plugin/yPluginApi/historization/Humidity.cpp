#include "stdafx.h"
#include "Humidity.h"
#include "../StandardValues.h"
#include "../StandardCapacities.h"
#include <shared/exception/InvalidParameter.hpp>
#include <shared/Log.h>

namespace shared
{
   namespace plugin
   {
      namespace yPluginApi
      {
         namespace historization
         {
            CHumidity::CHumidity(const std::string& keywordName,
                                 const EKeywordAccessMode& accessMode,
                                 const EMeasureType& measureType,
                                 typeInfo::CDoubleTypeInfo& additionalInfo)
               : CSingleHistorizableData<double>(keywordName,
                                                 CStandardCapacities::Humidity(),
                                                 accessMode,
                                                 0,
                                                 measureType,
                                                 additionalInfo)
            {
            }

            CHumidity::~CHumidity()
            {
            }


            double CHumidity::Normalize(double value)
            {
               if (value > 100)
                  return 100.0;
               if (value < 0)
                  return 0.0;
               return value;
            }
         }
      }
   }
} // namespace shared::plugin::yPluginApi::historization


