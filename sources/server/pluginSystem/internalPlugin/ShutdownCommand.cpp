#include "stdafx.h"
#include "ShutdownCommand.h"
#include <shared/exception/InvalidParameter.hpp>
#include <shared/Log.h>
#include <shared/plugin/yadomsApi/StandardUnits.h>

namespace pluginSystem {   namespace internalPlugin {

   DECLARE_ENUM_IMPLEMENTATION_CUSTOM(EShutdownCommand,
      ((ShutdowSystem)("shutdowSystem"))
   );

   const shared::plugin::yadomsApi::CStandardCapacity& ShutdownCapacity = shared::plugin::yadomsApi::CStandardCapacity("shutdown", shared::plugin::yadomsApi::CStandardUnits::NoUnits, shared::plugin::yadomsApi::EKeywordAccessMode::kGetSet, shared::plugin::yadomsApi::EKeywordDataType::kNoData);

   
   CShutdownCommand::CShutdownCommand(const std::string& keywordName)
      :shared::plugin::yadomsApi::historization::CHistorizableType<EShutdownCommand>(keywordName, ShutdownCapacity, "command")
   {
   }

   CShutdownCommand::~CShutdownCommand()
   {
   }









}} // namespace pluginSystem::internalPlugin