#include "stdafx.h"
#include "RemoteAtiWonderPlus.h"
#include <shared/plugin/yPluginApi/StandardUnits.h>

namespace xplrules
{
   namespace rfxLanXpl
   {
      namespace data
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

         DECLARE_CAPACITY(RemoteAtiWonderPlusCapacity, "RemoteAtiWonderPlus", shared::plugin::yPluginApi::CStandardUnits::NoUnits, shared::plugin::yPluginApi::EKeywordDataType::kNoData);


         CRemoteAtiWonderPlus::CRemoteAtiWonderPlus(const std::string& keywordName)
            : shared::plugin::yPluginApi::historization::CSingleHistorizableData<ERemoteAtiWonderPlusCodes>(keywordName,
                                                                                                            RemoteAtiWonderPlusCapacity(),
                                                                                                            shared::plugin::yPluginApi::EKeywordAccessMode::kGetSet)
         {
         }

         CRemoteAtiWonderPlus::~CRemoteAtiWonderPlus()
         {
         }
      }
   }
} // namespace xplrules::rfxLanXpl::data


