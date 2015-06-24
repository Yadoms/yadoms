#include "stdafx.h"
#include "DigitalIoCommand.h"
#include <shared/plugin/yPluginApi/StandardUnits.h>

namespace xplrules { namespace rfxLanXpl { namespace data {

DECLARE_ENUM_IMPLEMENTATION(EDigitalIoCommand,
   ((High))
   ((Low))
   ((Toggle))
);

const shared::plugin::yPluginApi::CStandardCapacity& RfxLanDigitalIoCommandCapacity = shared::plugin::yPluginApi::CStandardCapacity("digitalio", shared::plugin::yPluginApi::CStandardUnits::NoUnits, shared::plugin::yPluginApi::EKeywordDataType::kNoData);

   
CDigitalIoCommand::CDigitalIoCommand(const std::string& keywordName)
   :CSingleHistorizableData<EDigitalIoCommand>(keywordName, RfxLanDigitalIoCommandCapacity, shared::plugin::yPluginApi::EKeywordAccessMode::kGetSet)
{
}

CDigitalIoCommand::~CDigitalIoCommand()
{
}




















} } } // namespace xplrules::rfxLanXpl::data

