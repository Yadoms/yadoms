#include "DefaultIconSelector.h"

const std::string CDefaultIconSelector::IconsImgPath("/icons_img");
const std::string CDefaultIconSelector::BrightnessDecreasePath(IconsImgPath + "/BrightnessDecrease.png");
const std::string CDefaultIconSelector::BrightnessHighPath(IconsImgPath + "/BrightnessHigh.png");
const std::string CDefaultIconSelector::BrightnessLowPath(IconsImgPath + "/BrightnessLow.png");
const std::string CDefaultIconSelector::BrightnessMaxPath(IconsImgPath + "/BrightnessMax.png");
const std::string CDefaultIconSelector::BrightnessMinPath(IconsImgPath + "/BrightnessMin.png");
const std::string CDefaultIconSelector::DurationPath(IconsImgPath + "/Duration.png");
const std::string CDefaultIconSelector::FolderPath(IconsImgPath + "/Folder.png");
const std::string CDefaultIconSelector::MediaMutePath(IconsImgPath + "/MediaMute.png");
const std::string CDefaultIconSelector::MediaNextTrackPath(IconsImgPath + "/MediaNextTrack.png");
const std::string CDefaultIconSelector::MediaPlayPausePath(IconsImgPath + "/MediaPlayPause.png");
const std::string CDefaultIconSelector::MediaPrevTrackPath(IconsImgPath + "/MediaPrevTrack.png");
const std::string CDefaultIconSelector::MediaStopPath(IconsImgPath + "/MediaStop.png");
const std::string CDefaultIconSelector::MediaUnMutePath(IconsImgPath + "/MediaUnMute.png");
const std::string CDefaultIconSelector::MediaVolumeDownPath(IconsImgPath + "/MediaVolumeDown.png");
const std::string CDefaultIconSelector::MediaVolumeUpPath(IconsImgPath + "/MediaVolumeUp.png");
const std::string CDefaultIconSelector::OpenWebsitePath(IconsImgPath + "/OpenWebsite.png");
const std::string CDefaultIconSelector::SystemLaunchPath(IconsImgPath + "/SystemLaunch.png");
const std::string CDefaultIconSelector::ToggleMicroPath(IconsImgPath + "/ToggleMicro.png");
const std::string CDefaultIconSelector::ToggleMicroInactivePath(IconsImgPath + "/ToggleMicroInactive.png");
const std::string CDefaultIconSelector::MixerPostMessagePath(IconsImgPath + "/MixerPostMessage.png");
const std::string CDefaultIconSelector::MixerToggleSlowchatPath(IconsImgPath + "/MixerToggleSlowchat.png");
const std::string CDefaultIconSelector::MixerToggleSlowchatInactivePath(IconsImgPath + "/MixerToggleSlowchatInactive.png");
const std::string CDefaultIconSelector::ToggleHotkeyPath(IconsImgPath + "/ToggleHotkey.png");
const std::string CDefaultIconSelector::ToggleHotkeyOffPath(IconsImgPath + "/ToggleHotkeyOff.png");
const std::string CDefaultIconSelector::OnPath(IconsImgPath + "/On.png");
const std::string CDefaultIconSelector::OffPath(IconsImgPath + "/Off.png");
const std::string CDefaultIconSelector::PlusPath(IconsImgPath + "/Plus.png");
const std::string CDefaultIconSelector::MinusPath(IconsImgPath + "/Minus.png");
const std::string CDefaultIconSelector::ColorBrightnessPath(IconsImgPath + "/ColorBrightness.png");
const std::string CDefaultIconSelector::ColorTemperaturePath(IconsImgPath + "/ColorTemperature.png");
const std::string CDefaultIconSelector::ContrastPath(IconsImgPath + "/Contrast.png");
const std::string CDefaultIconSelector::BroadcastOnPath(IconsImgPath + "/BroadcastOn.png");
const std::string CDefaultIconSelector::BroadcastOffPath(IconsImgPath + "/BroadcastOff.png");
const std::string CDefaultIconSelector::LightOffPath(IconsImgPath + "/LightOff.png");
const std::string CDefaultIconSelector::LightColorPath(IconsImgPath + "/LightColor.png");
const std::string CDefaultIconSelector::PartyPopperPath(IconsImgPath + "/PartyPopper.png");
const std::string CDefaultIconSelector::SystemBackPath(IconsImgPath + "/SystemBack.png");

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
                           ((LightOff))
                           ((LightColor))
                           ((PartyPopper))
                           ((SystemBack))

);

std::vector<std::string> CDefaultIconSelector::getAllDefaultIconNames()
{
   const EDefaultIconType defaultIconType;
   return defaultIconType.getAllStrings();
}

std::string CDefaultIconSelector::getIconPath(std::string& pluginPath, int& iconNameIndex)
{
   std::string iconNamePath;
   std::string iconFullPath;
   switch (iconNameIndex)
   {
   case EDefaultIconType::kBrightnessDecreaseValue:
      iconNamePath = BrightnessDecreasePath;
      break;
   case EDefaultIconType::kBrightnessHighValue:
      iconNamePath =BrightnessHighPath;
      break;
   case EDefaultIconType::kBrightnessLowValue:
      iconNamePath = BrightnessLowPath;
      break;
   case EDefaultIconType::kBrightnessMaxValue:
      iconNamePath = BrightnessMaxPath;
      break;
   case EDefaultIconType::kBrightnessMinValue:
      iconNamePath = BrightnessMinPath;
      break;
   case EDefaultIconType::kDurationValue:
      iconNamePath = DurationPath;
      break;
   case EDefaultIconType::kFolderValue:
      iconNamePath = FolderPath;
      break;
   case EDefaultIconType::kMediaMuteValue:
      iconNamePath = MediaMutePath;
      break;
   case EDefaultIconType::kMediaNextTrackValue:
      iconNamePath = MediaNextTrackPath;
      break;
   case EDefaultIconType::kMediaPlayPauseValue:
      iconNamePath = MediaPlayPausePath;
      break;
   case EDefaultIconType::kMediaPrevTrackValue:
      iconNamePath = MediaPrevTrackPath;
      break;
   case EDefaultIconType::kMediaStopValue:
      iconNamePath = MediaStopPath;
      break;
   case EDefaultIconType::kMediaUnMuteValue:
      iconNamePath = MediaUnMutePath;
      break;
   case EDefaultIconType::kMediaVolumeDownValue:
      iconNamePath = MediaVolumeDownPath;
      break;
   case EDefaultIconType::kMediaVolumeUpValue:
      iconNamePath = MediaVolumeUpPath;
      break;
   case EDefaultIconType::kOpenWebsiteValue:
      iconNamePath = OpenWebsitePath;
      break;
   case EDefaultIconType::kSystemLaunchValue:
      iconNamePath = SystemLaunchPath;
      break;
   case EDefaultIconType::kToggleMicroValue:
      iconNamePath = ToggleMicroPath;
      break;
   case EDefaultIconType::kToggleMicroInactiveValue:
      iconNamePath = ToggleMicroInactivePath;
      break;
   case EDefaultIconType::kMixerPostMessageValue:
      iconNamePath = MixerPostMessagePath;
      break;
   case EDefaultIconType::kMixerToggleSlowchatValue:
      iconNamePath = MixerToggleSlowchatPath;
      break;
   case EDefaultIconType::kMixerToggleSlowchatInactiveValue:
      iconNamePath = MixerToggleSlowchatInactivePath;
      break;
   case EDefaultIconType::kToggleHotkeyValue:
      iconNamePath = ToggleHotkeyPath;
      break;
   case EDefaultIconType::kToggleHotkeyOffValue:
      iconNamePath = ToggleHotkeyOffPath;
      break;
   case EDefaultIconType::kOnValue:
      iconNamePath = OnPath;
      break;
   case EDefaultIconType::kOffValue:
      iconNamePath = OffPath;
      break;
   case EDefaultIconType::kPlusValue:
      iconNamePath = PlusPath;
      break;
   case EDefaultIconType::kMinusValue:
      iconNamePath = MinusPath;
      break;
   case EDefaultIconType::kColorBrightnessValue:
      iconNamePath = ColorBrightnessPath;
      break;
   case EDefaultIconType::kColorTemperatureValue:
      iconNamePath = ColorTemperaturePath;
      break;
   case EDefaultIconType::kContrastValue:
      iconNamePath = ContrastPath;
      break;
   case EDefaultIconType::kBroadcastOnValue:
      iconNamePath = BroadcastOnPath;
      break;
   case EDefaultIconType::kBroadcastOffValue:
      iconNamePath = BroadcastOffPath;
      break;
   case EDefaultIconType::kLightOffValue:
      iconNamePath = LightOffPath;
      break;
   case EDefaultIconType::kLightColorValue:
      iconNamePath = LightColorPath;
      break;
   case EDefaultIconType::kPartyPopperValue:
      iconNamePath = PartyPopperPath;
      break;
   case EDefaultIconType::kSystemBackValue:
      iconNamePath = SystemBackPath;
      break;

   default:
      throw std::runtime_error("geticonNamePath : icon path does not exist");
   }
   iconFullPath = pluginPath + iconNamePath;
   return iconFullPath;
}
