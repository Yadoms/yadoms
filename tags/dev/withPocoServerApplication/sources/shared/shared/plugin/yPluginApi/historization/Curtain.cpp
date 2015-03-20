#include "stdafx.h"
#include "Curtain.h"
#include "../StandardValues.h"


namespace shared { namespace plugin { namespace yPluginApi { namespace historization
{
   DECLARE_ENUM_IMPLEMENTATION(ECurtainCommand,
      ((Stop))
      ((Open))
      ((Close))
   );

   CCurtain::CCurtain(const std::string& keywordName)
      :CSingleHistorizableData<ECurtainCommand>(keywordName, CStandardCapacities::Curtain, EKeywordAccessMode::kGet, ECurtainCommand::kStop)
   {
   }

   CCurtain::~CCurtain()
   {
   }


} } } } // namespace shared::plugin::yPluginApi::historization

