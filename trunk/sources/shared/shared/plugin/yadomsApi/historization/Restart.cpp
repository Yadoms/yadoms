#include "stdafx.h"
#include "Restart.h"
#include <shared/exception/InvalidParameter.hpp>
#include <shared/Log.h>
#include <shared/plugin/yadomsApi/StandardUnits.h>
#include <shared/plugin/yadomsApi/StandardCapacities.h>

namespace shared { namespace plugin { namespace yadomsApi { namespace historization {

   DECLARE_ENUM_IMPLEMENTATION(ERestartCommand,
      ((Restart))
   );


   CRestart::CRestart(const std::string& keywordName)
		:CSingleHistorizableData<ERestartCommand>(keywordName, shared::plugin::yadomsApi::CStandardCapacities::Restart, "command", shared::plugin::yadomsApi::EKeywordAccessMode::kGetSet)
   {
   }

   CRestart::~CRestart()
   {
   }

 } } } } //namespace shared::plugin::yadomsApi::historization