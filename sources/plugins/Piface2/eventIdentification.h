#pragma once
#include <plugin_cpp_api/IPlugin.h>

// Shortcut to yPluginApi namespace
namespace yApi = shared::plugin::yPluginApi;

// Event IDs
enum
{
   kEvtIOStateReceived = yApi::IYPluginApi::kPluginFirstEventId // Always start from shared::event::CEventHandler::kUserFirstId
};
