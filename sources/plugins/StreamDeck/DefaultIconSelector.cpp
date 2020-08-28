#include "DefaultIconSelector.h"
const std::string IconsImgPath("/icons_img");
const std::string BrightnessDecreasePath(IconsImgPath +"/BrightnessDecrease.png");
const std::string BrightnessHighPath(IconsImgPath +"/BrightnessHigh.png");
const std::string BrightnessLowPath(IconsImgPath +"/BrightnessLow.png");
const std::string BrightnessMaxPath(IconsImgPath +"/BrightnessMax.png");
const std::string BrightnessMinPath(IconsImgPath +"/BrightnessMin.png");
const std::string DurationPath(IconsImgPath +"/Duration.png");
const std::string FolderPath(IconsImgPath +"/Folder.png");
const std::string MediaMutePath(IconsImgPath +"/MediaMute.png");
const std::string MediaNextTrackPath(IconsImgPath +"/MediaNextTrack.png");
const std::string MediaPlayPausePath(IconsImgPath +"/MediaPlayPause.png");
const std::string MediaPrevTrackPath(IconsImgPath +"/MediaPrevTrack.png");
const std::string MediaStopPath(IconsImgPath +"/MediaStop.png");
const std::string MediaUnMutePath(IconsImgPath +"/MediaUnMute.png");
const std::string MediaVolumeDownPath(IconsImgPath +"/MediaVolumeDown.png");
const std::string MediaVolumeUpPath(IconsImgPath +"/MediaVolumeUp.png");
const std::string OpenWebsitePath(IconsImgPath +"/OpenWebsite.png");
const std::string SystemLaunchPath(IconsImgPath +"/SystemLaunch.png");
const std::string ToggleMicroPath(IconsImgPath +"/ToggleMicro.png");
const std::string ToggleMicroInactivePath(IconsImgPath +"/ToggleMicroInactive.png");
const std::string MixerPostMessagePath(IconsImgPath +"/MixerPostMessage.png");
const std::string MixerToggleSlowchatPath(IconsImgPath +"/MixerToggleSlowchat.png");
const std::string MixerToggleSlowchatInactivePath(IconsImgPath +"/MixerToggleSlowchatInactive.png");
const std::string ToggleHotkeyPath(IconsImgPath +"/ToggleHotkey.png");
const std::string ToggleHotkeyOffPath(IconsImgPath +"/ToggleHotkeyOff.png");
const std::string OnPath(IconsImgPath +"/On.png");
const std::string OffPath(IconsImgPath +"/Off.png");
const std::string PlusPath(IconsImgPath +"/Plus.png");
const std::string MinusPath(IconsImgPath +"/Minus.png");
const std::string ColorBrightnessPath(IconsImgPath +"/ColorBrightness.png");
const std::string ColorTemperaturePath(IconsImgPath +"/ColorTemperature.png");
const std::string ContrastPath(IconsImgPath +"/Contrast.png");
const std::string BroadcastOnPath(IconsImgPath +"/BroadcastOn.png");
const std::string BroadcastOffPath(IconsImgPath +"/BroadcastOff.png");
const std::string LightOffPath(IconsImgPath +"/LightOff.png");
const std::string LightColorPath(IconsImgPath +"/LightColor.png");
const std::string PartyPopperPath(IconsImgPath +"/PartyPopper.png");
const std::string SystemBackPath(IconsImgPath +"/SystemBack.png");

DECLARE_ENUM_IMPLEMENTATION(EDefaultIconType,
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
                           ((LightColor))
                           ((PartyPopper))
                           ((SystemBack))

);
EIconSelectionMode CDefaultIconSelector::getIconSelectionMode(
   boost::shared_ptr<shared::plugin::yPluginApi::IExtraQuery>& extraQuery)
{
   return extraQuery->getData()->data()->get<std::string>(
             "iconSelectionMode.activeSectionText") == "Custom"
             ? kCustom
             : kDefault;
}

std::vector<std::string> CDefaultIconSelector::getAllDefaultIconNames()
{
   std::vector<std::string> defaultIcons;
   for (auto i = 0; i < EDefaultIconType::kSystemBack; i++)
   {
      auto eCurrentDefaultIcon = static_cast<EDefaultIconType>(i);
      defaultIcons.push_back(eCurrentDefaultIcon.toString());
   }
   return defaultIcons;
}
