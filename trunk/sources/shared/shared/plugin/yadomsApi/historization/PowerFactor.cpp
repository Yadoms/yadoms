#include "stdafx.h"
#include "PowerFactor.h"
#include "../StandardCapacities.h"

namespace shared { namespace plugin { namespace yadomsApi { namespace historization
{

   CPowerFactor::CPowerFactor(const std::string& keywordName, const EMeasureType& measureType)
      :CSingleHistorizableData<double>(keywordName, CStandardCapacities::Frequency, "powerFactor", 0.0, measureType)
   {
      //TODO : Seb, est-ce une capactié de type fréquence ?
   }

   CPowerFactor::~CPowerFactor()
   {
   }

   double CPowerFactor::Normalize(double value)
   {
      if (value > 1.0)
         return 1.0;
      if (value < -1.0)
         return -1.0;
      return value;
   }


} } } } // namespace shared::plugin::yadomsApi::historization

