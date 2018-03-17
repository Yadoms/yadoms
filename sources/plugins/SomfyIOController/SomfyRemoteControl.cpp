#include "stdafx.h"
#include "SomfyRemoteControl.h"
#include <shared/plugin/yPluginApi/StandardCapacities.h>
#include <shared/plugin/yPluginApi/StandardUnits.h>

DECLARE_ENUM_IMPLEMENTATION(ESomfyRemoteControlValues,
   ((Up))
   ((Down))
   ((My))
   ((Channel))
   ((Prog))
)


DECLARE_CAPACITY(SomfyRemoteControlCapacity, "somfyremotecontrol_capacity", shared::plugin::yPluginApi::CStandardUnits::NoUnit(), shared::plugin::yPluginApi::EKeywordDataType::kEnum);

   CRemoteControlValue::CRemoteControlValue(const std::string& keywordName)
   : CSingleHistorizableData<ESomfyRemoteControlValues>(keywordName,
                                                    SomfyRemoteControlCapacity(),
                                                    shared::plugin::yPluginApi::EKeywordAccessMode::kGetSet)
{
}

CRemoteControlValue::~CRemoteControlValue()
{
}

