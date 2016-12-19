#include "stdafx.h"
#include "RemoteAtiWonder2CmdTypeHistorizer.h"
#include <shared/plugin/yPluginApi/StandardUnits.h>
#include "RemoteAtiWonder2Capacity.h"

namespace rfxcomMessages {
   namespace specificHistorizers {

      DECLARE_ENUM_IMPLEMENTATION(ERemoteAtiWonder2TypesCodes,
         ((PC))
         ((AUX1))
         ((AUX2))
         ((AUX3))
         ((AUX4))
      );


      CRemoteAtiWonder2CmdTypeHistorizer::CRemoteAtiWonder2CmdTypeHistorizer(const std::string& keywordName)
         :CSingleHistorizableData<ERemoteAtiWonder2TypesCodes>(keywordName,
                                                               RemoteAtiWonder2Capacity(),
                                                               shared::plugin::yPluginApi::EKeywordAccessMode::kGetSet)
      {
      }

      CRemoteAtiWonder2CmdTypeHistorizer::~CRemoteAtiWonder2CmdTypeHistorizer()
      {
      }
   }
} // namespace rfxcomMessages::specificHistorizers

