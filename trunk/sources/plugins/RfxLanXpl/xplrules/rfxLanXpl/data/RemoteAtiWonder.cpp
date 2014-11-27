#include "stdafx.h"
#include "RemoteAtiWonder.h"
#include <shared/plugin/yadomsApi/StandardUnits.h>

namespace xplrules { namespace rfxLanXpl { namespace data {

   DECLARE_ENUM_IMPLEMENTATION(ERemoteAtiWonderCodes,
      ((A))
      ((B))
      ((Power))
      ((TV))
      ((DVD))
      ((InterrogrationPoint))
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
      ((SnapshotESC))
      ((C))
      ((Up))
      ((D))
      ((TVRADIO))
      ((Left))
      ((OK))
      ((Right))
      ((Down))
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
      ((V2))
      ((V2End))
      ((X))
      ((XEnd))
);
   
   const shared::plugin::yadomsApi::CStandardCapacity& RemoteAtiWonderCapacity = shared::plugin::yadomsApi::CStandardCapacity("RemoteAtiWonder", shared::plugin::yadomsApi::CStandardUnits::NoUnits, shared::plugin::yadomsApi::EKeywordDataType::kNoData);

   
   CRemoteAtiWonder::CRemoteAtiWonder(const std::string& keywordName)
      :shared::plugin::yadomsApi::historization::CSingleHistorizableData<ERemoteAtiWonderCodes>(keywordName, RemoteAtiWonderCapacity, shared::plugin::yadomsApi::EKeywordAccessMode::kGetSet)
   {
   }

   CRemoteAtiWonder::~CRemoteAtiWonder()
   {
   }




} } } // namespace xplrules::rfxLanXpl::data

