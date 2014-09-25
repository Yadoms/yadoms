#include "stdafx.h"
#include "DigitalIo.h"
#include <shared/exception/InvalidParameter.hpp>
#include <shared/Log.h>
#include <shared/plugin/yadomsApi/StandardUnits.h>

namespace xplrules { namespace rfxLanXpl { namespace data {

DECLARE_ENUM_IMPLEMENTATION_CUSTOM(EDigitalIoCommand,
   ((High)("high"))
   ((Low)("low"))
   ((Toggle)("toggle"))
);

const shared::plugin::yadomsApi::CStandardCapacity& RfxLanDigitalIoCapacity = shared::plugin::yadomsApi::CStandardCapacity("digitalio", shared::plugin::yadomsApi::CStandardUnits::NoUnits, shared::plugin::yadomsApi::kSet, shared::plugin::yadomsApi::kNoData);

   
CDigitalIo::CDigitalIo(const std::string& keywordName)
   :CHistorizableEnum<EDigitalIoCommand>(keywordName, RfxLanDigitalIoCapacity, "command")
{
}

CDigitalIo::~CDigitalIo()
{
}




















} } } // namespace xplrules::rfxLanXpl::data

