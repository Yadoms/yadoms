#include "stdafx.h"
#include "Restart.h"
#include <shared/exception/InvalidParameter.hpp>
#include <shared/Log.h>
#include <shared/plugin/yadomsApi/StandardUnits.h>
#include <shared/plugin/yadomsApi/StandardCapacities.h>

namespace shared { namespace plugin { namespace yadomsApi { namespace historization {

   DECLARE_ENUM_IMPLEMENTATION_CUSTOM(ERestartCommand,
      ((Restart)("restart"))
   );


   CRestart::CRestart(const std::string& keywordName)
		:CHistorizableType<ERestartCommand>(keywordName, shared::plugin::yadomsApi::CStandardCapacities::Restart, "command")
   {
   }

   CRestart::~CRestart()
   {
   }

 } } } } //namespace shared::plugin::yadomsApi::historization