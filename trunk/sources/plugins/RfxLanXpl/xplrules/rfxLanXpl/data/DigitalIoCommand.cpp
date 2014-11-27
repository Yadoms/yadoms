#include "stdafx.h"
#include "DigitalIoCommand.h"
#include <shared/plugin/yadomsApi/StandardUnits.h>

namespace xplrules { namespace rfxLanXpl { namespace data {

DECLARE_ENUM_IMPLEMENTATION(EDigitalIoCommand,
   ((High))
   ((Low))
   ((Toggle))
);

const shared::plugin::yadomsApi::CStandardCapacity& RfxLanDigitalIoCommandCapacity = shared::plugin::yadomsApi::CStandardCapacity("digitalio", shared::plugin::yadomsApi::CStandardUnits::NoUnits, shared::plugin::yadomsApi::EKeywordDataType::kNoData);

   
CDigitalIoCommand::CDigitalIoCommand(const std::string& keywordName)
   :CSingleHistorizableData<EDigitalIoCommand>(keywordName, RfxLanDigitalIoCommandCapacity, shared::plugin::yadomsApi::EKeywordAccessMode::kGetSet)
{
}

CDigitalIoCommand::~CDigitalIoCommand()
{
}




















} } } // namespace xplrules::rfxLanXpl::data

