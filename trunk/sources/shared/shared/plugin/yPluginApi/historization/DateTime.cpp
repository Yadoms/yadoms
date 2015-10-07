#include "stdafx.h"
#include "DateTime.h"
#include "../../../currentTime/Provider.h"
#include "../StandardCapacities.h"

namespace shared { namespace plugin { namespace yPluginApi { namespace historization
{

   CDateTime::CDateTime(const std::string& keywordName, const EKeywordAccessMode& accessMode, const EMeasureType& measureType)
      :CSingleHistorizableData<boost::posix_time::ptime>(keywordName, CStandardCapacities::DateTime, accessMode, currentTime::Provider::now(), measureType)
   {
   }

   CDateTime::~CDateTime()
   {
   }

} } } } // namespace shared::plugin::yPluginApi::historization

