#include "stdafx.h"
#include "Voltage.h"
#include "../StandardCapacities.h"

namespace shared { namespace plugin { namespace yadomsApi { namespace historization
{

   CVoltage::CVoltage(const std::string& keywordName, const EMeasureType& measureType)
      :CSingleHistorizableData<double>(keywordName, CStandardCapacities::Voltage, "voltage", 0.0, measureType)
   {
   }

   CVoltage::~CVoltage()
   {
   }


} } } } // namespace shared::plugin::yadomsApi::historization

