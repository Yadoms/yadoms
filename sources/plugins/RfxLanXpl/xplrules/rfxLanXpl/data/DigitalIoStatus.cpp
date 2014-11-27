#include "stdafx.h"
#include "DigitalIoStatus.h"
#include <shared/plugin/yadomsApi/StandardUnits.h>

namespace xplrules { namespace rfxLanXpl { namespace data {

DECLARE_ENUM_IMPLEMENTATION(EDigitalIoStatus,
   ((High))
   ((Low))
);

const shared::plugin::yadomsApi::CStandardCapacity& RfxLanDigitalIoStatusCapacity = shared::plugin::yadomsApi::CStandardCapacity("digitalio", shared::plugin::yadomsApi::CStandardUnits::NoUnits, shared::plugin::yadomsApi::EKeywordDataType::kNoData);

   
CDigitalIoStatus::CDigitalIoStatus(const std::string& keywordName)
   :CSingleHistorizableData<EDigitalIoStatus>(keywordName, RfxLanDigitalIoStatusCapacity, shared::plugin::yadomsApi::EKeywordAccessMode::kGet)
{
}

CDigitalIoStatus::~CDigitalIoStatus()
{
}




















} } } // namespace xplrules::rfxLanXpl::data

