#include "stdafx.h"
#include "DateTime.h"
#include "../StandardCapacities.h"

namespace shared { namespace plugin { namespace yPluginApi { namespace historization
{

   CDateTime::CDateTime(const std::string& keywordName, const EKeywordAccessMode& accessMode, const EMeasureType& measureType)
      :CSingleHistorizableData<boost::posix_time::ptime>(keywordName, CStandardCapacities::DateTime, accessMode, boost::posix_time::second_clock::universal_time(), measureType)
   {
   }

   CDateTime::~CDateTime()
   {
   }

} } } } // namespace shared::plugin::yPluginApi::historization

