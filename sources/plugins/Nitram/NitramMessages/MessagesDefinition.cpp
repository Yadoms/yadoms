#pragma once

#include <shared/plugin/yPluginApi/IYPluginApi.h>
#include "MessagesDefinition.h"
#include "INitramMessage.h"

namespace nitramMessages
{
   const std::vector<boost::shared_ptr<const yApi::historization::IHistorizable>> nitramMessages::INitramMessage::NoKeywords;
} // namespace nitramMessages