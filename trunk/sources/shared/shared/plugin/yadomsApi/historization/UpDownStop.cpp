#include "stdafx.h"
#include "UpDownStop.h"
#include "../StandardValues.h"


namespace shared { namespace plugin { namespace yadomsApi { namespace historization
{
   DECLARE_ENUM_IMPLEMENTATION(EUpDownStopCommand,
      ((Stop))
      ((Up))
      ((Down))
   );

   CUpDownStop::CUpDownStop(const std::string& keywordName, const EKeywordAccessMode& accessMode)
      :CSingleHistorizableData<EUpDownStopCommand>(keywordName, CStandardCapacities::UpDownStop, accessMode, EUpDownStopCommand::kStop)
   {
   }

   CUpDownStop::~CUpDownStop()
   {
   }


} } } } // namespace shared::plugin::yadomsApi::historization

