#include "stdafx.h"
#include "RemoteAtiWonder2CmdHistorizer.h"
#include "RemoteAtiWonder2Capacity.h"

namespace rfxcomMessages
{
   namespace specificHistorizers
   {
      DECLARE_ENUM_IMPLEMENTATION(ERemoteAtiWonder2Codes,
         ((A))
         ((B))
         ((power))
         ((TV))
         ((DVD))
         ((InterrogrationPoint))
         ((ReversedInterrogrationPoint))
         ((Drag))
         ((VolumeUp))
         ((VolumeDown))
         ((Mute))
         ((ChannelUp))
         ((ChannelDown))
         ((Key1))
         ((Key2))
         ((Key3))
         ((Key4))
         ((Key5))
         ((Key6))
         ((Key7))
         ((Key8))
         ((Key9))
         ((txt))
         ((Key0))
         ((OpenSetupMenu))
         ((C))
         ((Up))
         ((D))
         ((TvRadio))
         ((Left))
         ((OK))
         ((Right))
         ((MaxRestoreWindow))
         ((E))
         ((Down))
         ((F))
         ((Rewind))
         ((Play))
         ((Fastforward))
         ((Record))
         ((Stop))
         ((Pause))
         ((ATI))
         ((PC))
         ((AUX1))
         ((AUX2))
         ((AUX3))
         ((AUX4))
         ((CursorLeft))
         ((CursorRight))
         ((CursorUp))
         ((CursorDown))
         ((CursorUpLeft))
         ((CursorUpRight))
         ((CursorDownRight))
         ((CursorDownLeft))
         ((LeftMouseButton))
         ((RightMouseButton))
      );


      CRemoteAtiWonder2CmdHistorizer::CRemoteAtiWonder2CmdHistorizer(const std::string& keywordName)
         : CSingleHistorizableData<ERemoteAtiWonder2Codes>(keywordName,
                                                           RemoteAtiWonder2Capacity(),
                                                           shared::plugin::yPluginApi::EKeywordAccessMode::kGetSet)
      {
      }

      CRemoteAtiWonder2CmdHistorizer::~CRemoteAtiWonder2CmdHistorizer()
      {
      }
   }
} // namespace rfxcomMessages::specificHistorizers


