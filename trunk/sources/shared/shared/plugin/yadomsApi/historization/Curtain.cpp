#include "stdafx.h"
#include "Curtain.h"
#include "../StandardValues.h"


namespace shared { namespace plugin { namespace yadomsApi { namespace historization
{
   DECLARE_ENUM_IMPLEMENTATION(ECurtainCommand,
      (Stop)
      (Open)
      (Close)
   );

   CCurtain::CCurtain(const std::string& keywordName)
      :CSingleHistorizableData<ECurtainCommand>(keywordName, CStandardCapacities::Curtain, "cmd", ECurtainCommand(ECurtainCommand::kStop) )
   {
   }

   CCurtain::~CCurtain()
   {
   }


} } } } // namespace shared::plugin::yadomsApi::historization

