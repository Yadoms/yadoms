#include "stdafx.h"
#include "RemotePC.h"
#include <shared/plugin/yPluginApi/StandardUnits.h>

namespace xplrules
{
   namespace rfxLanXpl
   {
      namespace data
      {
         DECLARE_ENUM_IMPLEMENTATION(ERemotePCCodes,
            ((Key0))
            ((Key8))
            ((Key4))
            ((Rewind))
            ((Info))
            ((ChannelUp))
            ((Key2))
            ((Ent))
            ((VolumeUp))
            ((Key6))
            ((Stop))
            ((Pause))
            ((CursorLeft))
            ((CursorRight))
            ((CursorUp))
            ((CursorDown))
            ((CursorUpLeft))
            ((CursorUpRight))
            ((CursorDownRight))
            ((CursorDownLeft))
            ((LeftMouse))
            ((LeftMouseEnd))
            ((Drag))
            ((RightMouse))
            ((RightMouseEnd))
            ((Key1))
            ((Key9))
            ((Mute))
            ((Key5))
            ((Play))
            ((MenuForward))
            ((Fast))
            ((AandB))
            ((ChannelDown))
            ((Key3))
            ((Exit))
            ((MP3))
            ((DVD))
            ((CD))
            ((PCShift4))
            ((Shift5))
            ((ShiftEnt))
            ((ShiftTeletext))
            ((Text))
            ((ShiftText))
            ((VolumeDown))
            ((Key7))
            ((Teletext))
            ((Record))
         );

         DECLARE_CAPACITY(RemotePCCapacity, "RemotePC", shared::plugin::yPluginApi::CStandardUnits::NoUnits, shared::plugin::yPluginApi::EKeywordDataType::kNoData);


         CRemotePC::CRemotePC(const std::string& keywordName)
            : shared::plugin::yPluginApi::historization::CSingleHistorizableData<ERemotePCCodes>(keywordName,
                                                                                                 RemotePCCapacity(),
                                                                                                 shared::plugin::yPluginApi::EKeywordAccessMode::kGetSet)
         {
         }

         CRemotePC::~CRemotePC()
         {
         }
      }
   }
} // namespace xplrules::rfxLanXpl::data


