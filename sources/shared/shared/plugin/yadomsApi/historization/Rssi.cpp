#include "stdafx.h"
#include "Rssi.h"
#include "../StandardCapacities.h"


namespace shared { namespace plugin { namespace yadomsApi { namespace historization
{

   CRssi::CRssi(const std::string& keywordName, const EMeasureType& measureType)
      :CSingleHistorizableData<int>(keywordName, CStandardCapacities::Rssi, "rssi", 0, measureType)
   {
   }

   CRssi::~CRssi()
   {
   }


   int CRssi::Normalize(int rssi)
   {
      if (rssi > 100)
         return 100;
      if (rssi < 0)
         return 0;
      return rssi;
   }


} } } } // namespace shared::plugin::yadomsApi::historization

