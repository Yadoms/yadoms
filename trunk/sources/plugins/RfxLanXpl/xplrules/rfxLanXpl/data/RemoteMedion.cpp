#include "stdafx.h"
#include "RemoteMedion.h"
#include <shared/plugin/yadomsApi/StandardUnits.h>

namespace xplrules { namespace rfxLanXpl { namespace data {

   DECLARE_ENUM_IMPLEMENTATION(ERemoteMedionCodes,
      ((Mute))
      ((B))
      ((Power))
      ((TV))
      ((DVD))
      ((Photo))
      ((Music))
      ((Drag))
      ((VolumeDown))
      ((VolumeUp))
      ((Mute2))
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
      ((DVDMenu))
      ((Up))
      ((Setup))
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
   
   const shared::plugin::yadomsApi::CStandardCapacity& RemoteMedionCapacity = shared::plugin::yadomsApi::CStandardCapacity("RemoteMedion", shared::plugin::yadomsApi::CStandardUnits::NoUnits, shared::plugin::yadomsApi::EKeywordDataType::kNoData);

   
   CRemoteMedion::CRemoteMedion(const std::string& keywordName)
      :shared::plugin::yadomsApi::historization::CSingleHistorizableData<ERemoteMedionCodes>(keywordName, RemoteMedionCapacity, shared::plugin::yadomsApi::EKeywordAccessMode::kGetSet)
   {
   }

   CRemoteMedion::~CRemoteMedion()
   {
   }




} } } // namespace xplrules::rfxLanXpl::data

