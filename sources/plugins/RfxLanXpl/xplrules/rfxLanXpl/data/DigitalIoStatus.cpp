#include "stdafx.h"
#include "DigitalIoStatus.h"
#include <shared/plugin/yPluginApi/StandardUnits.h>

namespace xplrules
{
   namespace rfxLanXpl
   {
      namespace data
      {
         DECLARE_ENUM_IMPLEMENTATION(EDigitalIoStatus,
            ((High))
            ((Low))
         );

         static const shared::plugin::yPluginApi::CStandardCapacity RfxLanDigitalIoStatusCapacity("digitalio",
                                                                                                  shared::plugin::yPluginApi::CStandardUnits::NoUnits,
                                                                                                  shared::plugin::yPluginApi::EKeywordDataType::kNoData);


         CDigitalIoStatus::CDigitalIoStatus(const std::string& keywordName)
            : CSingleHistorizableData<EDigitalIoStatus>(keywordName,
                                                        RfxLanDigitalIoStatusCapacity,
                                                        shared::plugin::yPluginApi::EKeywordAccessMode::kGet)
         {
         }

         CDigitalIoStatus::~CDigitalIoStatus()
         {
         }
      }
   }
} // namespace xplrules::rfxLanXpl::data


