#include "stdafx.h"
#include "Temperature.h"
#include "../StandardCapacities.h"

namespace shared { namespace plugin { namespace yadomsApi { namespace historization
{

   CTemperature::CTemperature(const std::string& keywordName, const EMeasureType& measureType)
      :CSingleHistorizableData<double>(keywordName, CStandardCapacities::Temperature, "temperature", 0.0, measureType)
   {
   }

   CTemperature::~CTemperature()
   {
   }

} } } } // namespace shared::plugin::yadomsApi::historization

