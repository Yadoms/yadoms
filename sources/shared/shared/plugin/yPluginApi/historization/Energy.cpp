#include "stdafx.h"
#include "Energy.h"
#include "../StandardCapacities.h"

namespace shared
{
   namespace plugin
   {
      namespace yPluginApi
      {
         namespace historization
         {
            CEnergy::CEnergy(const std::string& keywordName,
                             const EKeywordAccessMode& accessMode,
                             const EMeasureType& measureType,
                             typeInfo::CDoubleTypeInfo& additionalInfo)
               : CSingleHistorizableData<Poco::Int64>(keywordName,
                                                      CStandardCapacities::Energy(),
                                                      accessMode,
                                                      0,
                                                      measureType,
                                                      additionalInfo)
            {
            }

            CEnergy::~CEnergy()
            {
            }

            CEnergyDouble::CEnergyDouble(const std::string& keywordName,
                                         const EKeywordAccessMode& accessMode,
                                         const EMeasureType& measureType,
                                         typeInfo::CDoubleTypeInfo& additionalInfo)
               : CSingleHistorizableData<double>(keywordName,
                                                 CStandardCapacities::Energy(),
                                                 accessMode,
                                                 0,
                                                 measureType,
                                                 additionalInfo)
            {
            }

            CEnergyDouble::~CEnergyDouble()
            {
            }
         }
      }
   }
} // namespace shared::plugin::yPluginApi::historization


