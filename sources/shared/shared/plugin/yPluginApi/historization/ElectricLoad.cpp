#include "stdafx.h"
#include "ElectricLoad.h"
#include "../StandardCapacities.h"


namespace shared
{
   namespace plugin
   {
      namespace yPluginApi
      {
         namespace historization
         {
            CElectricLoad::CElectricLoad(const std::string& keywordName,
                                         const EKeywordAccessMode& accessMode,
                                         const EMeasureType& measureType,
                                         typeInfo::CDoubleTypeInfo& additionalInfo)
               : CSingleHistorizableData<double>(keywordName,
                                                 CStandardCapacities::ElectricLoad(),
                                                 accessMode,
                                                 0.0,
                                                 measureType,
                                                 additionalInfo)
            {
            }

            CElectricLoad::~CElectricLoad()
            {
            }
         }
      }
   }
} // namespace shared::plugin::yPluginApi::historization


