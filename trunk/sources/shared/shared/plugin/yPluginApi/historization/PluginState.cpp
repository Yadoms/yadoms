#include "stdafx.h"
#include "PluginState.h"
#include "../StandardValues.h"
#include <shared/exception/InvalidParameter.hpp>
#include <shared/DataContainer.h>
#include <shared/StringExtension.h>


namespace shared { namespace plugin { namespace yPluginApi { namespace historization
{

DECLARE_ENUM_IMPLEMENTATION(EPluginState,
   ((Unknown))
   ((Error))
   ((Stopped))
   ((Running))
   ((Custom))
);

CPluginState::CPluginState(const std::string& keywordName, const EKeywordAccessMode& accessMode)
   :CSingleHistorizableData<EPluginState>(keywordName, CStandardCapacities::Text, accessMode, EPluginState::kUnknown, EMeasureType::kAbsolute)
{
}

CPluginState::~CPluginState()
{
}

} } } } // namespace shared::plugin::yPluginApi::historization

