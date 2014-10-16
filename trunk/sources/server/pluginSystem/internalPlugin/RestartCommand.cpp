#include "stdafx.h"
#include "RestartCommand.h"
#include <shared/exception/InvalidParameter.hpp>
#include <shared/Log.h>
#include <shared/plugin/yadomsApi/StandardUnits.h>

namespace pluginSystem { namespace internalPlugin {

   DECLARE_ENUM_IMPLEMENTATION_CUSTOM(ERestartCommand,
      ((RestartSystem)("restartSystem"))
   );

   const shared::plugin::yadomsApi::CStandardCapacity& RestartCapacity = shared::plugin::yadomsApi::CStandardCapacity("restart", shared::plugin::yadomsApi::CStandardUnits::NoUnits, shared::plugin::yadomsApi::EKeywordAccessMode::kSet, shared::plugin::yadomsApi::EKeywordDataType::kNoData);

   CRestartCommand::CRestartCommand(const std::string& keywordName)
      :shared::plugin::yadomsApi::historization::CHistorizableType<ERestartCommand>(keywordName, RestartCapacity, "command")
   {
   }

   CRestartCommand::~CRestartCommand()
   {
   }

}} // namespace pluginSystem::internalPlugin