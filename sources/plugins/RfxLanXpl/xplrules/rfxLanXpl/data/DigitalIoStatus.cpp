#include "stdafx.h"
#include "DigitalIoStatus.h"
#include <shared/exception/InvalidParameter.hpp>
#include <shared/Log.h>
#include <shared/plugin/yadomsApi/StandardUnits.h>

namespace xplrules { namespace rfxLanXpl { namespace data {

DECLARE_ENUM_IMPLEMENTATION_CUSTOM(EDigitalIoStatus,
   ((High)("high"))
   ((Low)("low"))
);

const shared::plugin::yadomsApi::CStandardCapacity& RfxLanDigitalIoStatusCapacity = shared::plugin::yadomsApi::CStandardCapacity("digitalio", shared::plugin::yadomsApi::CStandardUnits::NoUnits, shared::plugin::yadomsApi::EKeywordAccessMode::kGet, shared::plugin::yadomsApi::EKeywordDataType::kNoData);

   
CDigitalIoStatus::CDigitalIoStatus(const std::string& keywordName)
   :CHistorizableEnum<EDigitalIoStatus>(keywordName, RfxLanDigitalIoStatusCapacity, "status")
{
}

CDigitalIoStatus::~CDigitalIoStatus()
{
}




















} } } // namespace xplrules::rfxLanXpl::data

