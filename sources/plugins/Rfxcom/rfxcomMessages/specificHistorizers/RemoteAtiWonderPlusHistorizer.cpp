#include "stdafx.h"
#include "RemoteAtiWonderPlusHistorizer.h"
#include <shared/plugin/yPluginApi/StandardUnits.h>

namespace rfxcomMessages
{
   namespace specificHistorizers
   {
      DECLARE_ENUM_IMPLEMENTATION(ERemoteAtiWonderPlusCodes,
         ((A))
         ((B))
         ((Power))
         ((TV))
         ((DVD))
         ((InterrogationPoint))
         ((Guide))
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
         ((Txt))
         ((Key0))
         ((OpenSetupMenu))
         ((C))
         ((Up))
         ((D))
         ((FM))
         ((Left))
         ((OK))
         ((Right))
         ((MaxRestoreWindow))
         ((E))
         ((V))
         ((F))
         ((Rewind))
         ((Play))
         ((FastForward))
         ((Record))
         ((Stop))
         ((Pause))
         ((TV2))
         ((Clock))
         ((TV3))
         ((VCR))
         ((RADIO))
         ((TVPreview))
         ((ChannelList))
         ((VideoDesktop))
         ((Red))
         ((Green))
         ((Yellow))
         ((Blue))
         ((RenameTab))
         ((AcquireImage))
         ((EditImage))
         ((FullScreen))
         ((DVDAudio))
         ((CursorLeft))
         ((CursorRight))
         ((CursorUp))
         ((CursorDown))
         ((CursorUpLeft))
         ((CursorUpRight))
         ((CursorDownRight))
         ((CursorDownLeft))
         ((LeftMouseButton))
         ((VEnd))
         ((RightMouseButton))
         ((XEnd))
      )

      static const shared::plugin::yPluginApi::CStandardCapacity RemoteAtiWonderPlusCapacity("RemoteAtiWonderPlus",
                                                                                             shared::plugin::yPluginApi::CStandardUnits::NoUnits,
                                                                                             shared::plugin::yPluginApi::EKeywordDataType::kNoData);


      CRemoteAtiWonderPlusHistorizer::CRemoteAtiWonderPlusHistorizer(const std::string& keywordName)
         : CSingleHistorizableData<ERemoteAtiWonderPlusCodes>(keywordName,
                                                              RemoteAtiWonderPlusCapacity,
                                                              shared::plugin::yPluginApi::EKeywordAccessMode::kGetSet)
      {
      }

      CRemoteAtiWonderPlusHistorizer::~CRemoteAtiWonderPlusHistorizer()
      {
      }
   }
} // namespace rfxcomMessages::specificHistorizers


