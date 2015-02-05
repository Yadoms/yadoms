#include "stdafx.h"
#include "Voltage.h"
#include "../StandardCapacities.h"

namespace shared { namespace plugin { namespace yPluginApi { namespace historization
{

   CVoltage::CVoltage(const std::string& keywordName, const EKeywordAccessMode& accessMode, const EMeasureType& measureType)
      :CSingleHistorizableData<double>(keywordName, CStandardCapacities::Voltage, accessMode, 0.0, measureType)
   {
   }

   CVoltage::~CVoltage()
   {
   }


} } } } // namespace shared::plugin::yPluginApi::historization

