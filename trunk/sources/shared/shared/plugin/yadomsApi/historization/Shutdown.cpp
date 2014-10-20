#include "stdafx.h"
#include "Shutdown.h"
#include <shared/exception/InvalidParameter.hpp>
#include <shared/Log.h>
#include <shared/plugin/yadomsApi/StandardUnits.h>
#include <shared/plugin/yadomsApi/StandardCapacities.h>

namespace shared { namespace plugin { namespace yadomsApi { namespace historization {

   DECLARE_ENUM_IMPLEMENTATION_CUSTOM(EShutdownCommand,
      ((Shutdown)("shutdown"))
   );


   
   CShutdown::CShutdown(const std::string& keywordName)
		:CHistorizableType<EShutdownCommand>(keywordName, shared::plugin::yadomsApi::CStandardCapacities::Shutdown, "command")
   {
   }

   CShutdown::~CShutdown()
   {
   }


 } } } } //namespace shared::plugin::yadomsApi::historization