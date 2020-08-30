#pragma once
#include "shared/plugin/yPluginApi/IExtraQuery.h"
#include <shared/enumeration/EnumHelpers.hpp>

enum EIconSelectionMode
{
   kCustom = 0,
   kDefault
};

DECLARE_ENUM_HEADER(EDefaultIconType,
                           ((BrightnessDecrease))
                           ((BrightnessHigh))
                           ((BrightnessLow))
                           ((BrightnessMax))
                           ((BrightnessMin))
                           ((Duration))
                           ((Folder))
                           ((MediaMute))
                           ((MediaNextTrack))
                           ((MediaPlayPause))
                           ((MediaPrevTrack))
                           ((MediaStop))
                           ((MediaUnMute))
                           ((MediaVolumeDown))
                           ((MediaVolumeUp))
                           ((OpenWebsite))
                           ((SystemLaunch))
                           ((ToggleMicro))
                           ((ToggleMicroInactive))
                           ((MixerPostMessage))
                           ((MixerToggleSlowchat))
                           ((MixerToggleSlowchatInactive))
                           ((ToggleHotkey))
                           ((ToggleHotkeyOff))
                           ((On))
                           ((Off))
                           ((Plus))
                           ((Minus))
                           ((ColorBrightness))
                           ((ColorTemperature))
                           ((Contrast))
                           ((BroadcastOn))
                           ((BroadcastOff))
                           ((LightOff))
                           ((LightColor))
                           ((PartyPopper))
                           ((SystemBack))
);

class CDefaultIconSelector final
{
public:
   CDefaultIconSelector() = default;;
   virtual ~CDefaultIconSelector() = default;

   static EIconSelectionMode getIconSelectionMode(
      boost::shared_ptr<shared::plugin::yPluginApi::IExtraQuery>& extraQuery);

   static std::vector<std::string> getAllDefaultIconNames();

   static std::string getIconPath(std::string& pluginPath, int& iconNameIndex);
private:
   static const std::string IconsImgPath;
   
};
