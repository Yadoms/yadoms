#pragma once
#include <shared/plugin/yPluginApi/IYPluginApi.h>

// Shortcut to yPluginApi namespace
namespace yApi = shared::plugin::yPluginApi;

enum
{
   kEvtIOStateReceived = yApi::IYPluginApi::kPluginFirstEventId // Always start from shared::event::CEventHandler::kUserFirstId
};