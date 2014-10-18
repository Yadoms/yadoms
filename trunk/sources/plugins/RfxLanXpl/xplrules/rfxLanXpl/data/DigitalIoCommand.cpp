#include "stdafx.h"
#include "DigitalIoCommand.h"
#include <shared/exception/InvalidParameter.hpp>
#include <shared/Log.h>
#include <shared/plugin/yadomsApi/StandardUnits.h>

namespace xplrules { namespace rfxLanXpl { namespace data {

DECLARE_ENUM_IMPLEMENTATION_CUSTOM(EDigitalIoCommand,
   ((High)("high"))
   ((Low)("low"))
   ((Toggle)("toggle"))
);

const shared::plugin::yadomsApi::CStandardCapacity& RfxLanDigitalIoCommandCapacity = shared::plugin::yadomsApi::CStandardCapacity("digitalio", shared::plugin::yadomsApi::CStandardUnits::NoUnits, shared::plugin::yadomsApi::EKeywordAccessMode::kGetSet, shared::plugin::yadomsApi::EKeywordDataType::kNoData);

   
CDigitalIoCommand::CDigitalIoCommand(const std::string& keywordName)
   :CHistorizableEnum<EDigitalIoCommand>(keywordName, RfxLanDigitalIoCommandCapacity, "command")
{
}

CDigitalIoCommand::~CDigitalIoCommand()
{
}




















} } } // namespace xplrules::rfxLanXpl::data

