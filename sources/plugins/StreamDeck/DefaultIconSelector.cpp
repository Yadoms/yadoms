#include "DefaultIconSelector.h"
const std::string IconsImgPath("/icons_img");

DECLARE_ENUM_IMPLEMENTATION(EDefaultIconType,
                            ((BrightnessDecrease)(IconsImgPath + "/BrightnessDecrease.png"))
                            ((BrightnessHigh)(IconsImgPath + "/BrightnessHigh.png"))
                            ((BrightnessLow)(IconsImgPath + "/BrightnessLow.png"))
                            ((BrightnessMax)(IconsImgPath + "/BrightnessMax.png"))
                            ((BrightnessMin)(IconsImgPath + "/BrightnessMin.png"))
                            ((Duration)(IconsImgPath + "/Duration.png"))
                            ((Folder)(IconsImgPath + "/Folder.png"))
                            ((MediaMute)(IconsImgPath + "/MediaMute.png"))
                            ((MediaNextTrack)(IconsImgPath + "/MediaNextTrack.png"))
                            ((MediaPlayPause)(IconsImgPath + "/MediaPlayPause.png"))
                            ((MediaPrevTrack)(IconsImgPath + "/MediaPrevTrack.png"))
                            ((MediaStop)(IconsImgPath + "/MediaStop.png"))
                            ((MediaUnMute)(IconsImgPath + "/MediaUnMute.png"))
                            ((MediaVolumeDown)(IconsImgPath + "/MediaVolumeDown.png"))
                            ((MediaVolumeUp)(IconsImgPath + "/MediaVolumeUp.png"))
                            ((OpenWebsite)(IconsImgPath + "/OpenWebsite.png"))
                            ((SystemLaunch)(IconsImgPath + "/SystemLaunch.png"))
                            ((ToggleMicro)(IconsImgPath + "/ToggleMicro.png"))
                            ((ToggleMicroInactive)(IconsImgPath + "/ToggleMicroInactive.png"))
                            ((MixerPostMessage)(IconsImgPath + "/MixerPostMessage.png"))
                            ((MixerToggleSlowchat)(IconsImgPath + "/MixerToggleSlowchat.png"))
                            ((MixerToggleSlowchatInactive)(IconsImgPath + "/MixerToggleSlowchatInactive.png"))
                            ((ToggleHotkey)(IconsImgPath + "/ToggleHotkey.png"))
                            ((ToggleHotkeyOff)(IconsImgPath + "/ToggleHotkeyOff.png"))
                            ((On)(IconsImgPath + "/On.png"))
                            ((Off)(IconsImgPath + "/Off.png"))
                            ((Plus)(IconsImgPath + "/Plus.png"))
                            ((Minus)(IconsImgPath + "/Minus.png"))
                            ((ColorBrightness)(IconsImgPath + "/ColorBrightness.png"))
                            ((ColorTemperature)(IconsImgPath + "/ColorTemperature.png"))
                            ((Contrast)(IconsImgPath + "/Contrast.png"))
                            ((BroadcastOn)(IconsImgPath + "/BroadcastOn.png"))
                            ((BroadcastOff)(IconsImgPath + "/BroadcastOff.png"))
                            ((LightOff)(IconsImgPath + "/LightOff.png"))
                            ((LightColor)(IconsImgPath + "/LightColor.png"))
                            ((PartyPopper)(IconsImgPath + "/PartyPopper.png"))
                            ((SystemBack)(IconsImgPath + "/SystemBack.png"))

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

std::string CDefaultIconSelector::getIconPath(std::string& pluginPath, int& iconNameIndex)
{
   std::string iconPath;
   switch (iconNameIndex)
   {
   case EDefaultIconType::kBrightnessDecreaseValue:
      iconPath = pluginPath + EDefaultIconType::kBrightnessDecrease.toString();
      break;
   case EDefaultIconType::kBrightnessHighValue:
      iconPath = pluginPath + EDefaultIconType::kBrightnessHigh.toString();
      break;
   case EDefaultIconType::kBrightnessLowValue:
      iconPath = pluginPath + EDefaultIconType::kBrightnessLow.toString();
      break;
   case EDefaultIconType::kBrightnessMaxValue:
      iconPath = pluginPath + EDefaultIconType::kBrightnessMax.toString();
      break;
   case EDefaultIconType::kBrightnessMinValue:
      iconPath = pluginPath + EDefaultIconType::kBrightnessMin.toString();
      break;
   case EDefaultIconType::kDurationValue:
      iconPath = pluginPath + EDefaultIconType::kDuration.toString();
      break;
   case EDefaultIconType::kFolderValue:
      iconPath = pluginPath + EDefaultIconType::kFolder.toString();
      break;
   case EDefaultIconType::kMediaMuteValue:
      iconPath = pluginPath + EDefaultIconType::kMediaMute.toString();
      break;
   case EDefaultIconType::kMediaNextTrackValue:
      iconPath = pluginPath + EDefaultIconType::kMediaNextTrack.toString();
      break;
   case EDefaultIconType::kMediaPlayPauseValue:
      iconPath = pluginPath + EDefaultIconType::kMediaPlayPause.toString();
      break;
   case EDefaultIconType::kMediaPrevTrackValue:
      iconPath = pluginPath + EDefaultIconType::kMediaPrevTrack.toString();
      break;
   case EDefaultIconType::kMediaStopValue:
      iconPath = pluginPath + EDefaultIconType::kMediaStop.toString();
      break;
   case EDefaultIconType::kMediaUnMuteValue:
      iconPath = pluginPath + EDefaultIconType::kMediaUnMute.toString();
      break;
   case EDefaultIconType::kMediaVolumeDownValue:
      iconPath = pluginPath + EDefaultIconType::kMediaVolumeDown.toString();
      break;
   case EDefaultIconType::kMediaVolumeUpValue:
      iconPath = pluginPath + EDefaultIconType::kMediaVolumeUp.toString();
      break;
   case EDefaultIconType::kOpenWebsiteValue:
      iconPath = pluginPath + EDefaultIconType::kOpenWebsite.toString();
      break;
   case EDefaultIconType::kSystemLaunchValue:
      iconPath = pluginPath + EDefaultIconType::kSystemLaunch.toString();
      break;
   case EDefaultIconType::kToggleMicroValue:
      iconPath = pluginPath + EDefaultIconType::kToggleMicro.toString();
      break;
   case EDefaultIconType::kToggleMicroInactiveValue:
      iconPath = pluginPath + EDefaultIconType::kToggleMicroInactive.toString();
      break;
   case EDefaultIconType::kMixerPostMessageValue:
      iconPath = pluginPath + EDefaultIconType::kMixerPostMessage.toString();
      break;
   case EDefaultIconType::kMixerToggleSlowchatValue:
      iconPath = pluginPath + EDefaultIconType::kMixerToggleSlowchat.toString();
      break;
   case EDefaultIconType::kMixerToggleSlowchatInactiveValue:
      iconPath = pluginPath + EDefaultIconType::kMixerToggleSlowchatInactive.toString();
      break;
   case EDefaultIconType::kToggleHotkeyValue:
      iconPath = pluginPath + EDefaultIconType::kToggleHotkey.toString();
      break;
   case EDefaultIconType::kToggleHotkeyOffValue:
      iconPath = pluginPath + EDefaultIconType::kToggleHotkeyOff.toString();
      break;
   case EDefaultIconType::kOnValue:
      iconPath = pluginPath + EDefaultIconType::kOn.toString();
      break;
   case EDefaultIconType::kOffValue:
      iconPath = pluginPath + EDefaultIconType::kOff.toString();
      break;
   case EDefaultIconType::kPlusValue:
      iconPath = pluginPath + EDefaultIconType::kPlus.toString();
      break;
   case EDefaultIconType::kMinusValue:
      iconPath = pluginPath + EDefaultIconType::kMinus.toString();
      break;
   case EDefaultIconType::kColorBrightnessValue:
      iconPath = pluginPath + EDefaultIconType::kColorBrightness.toString();
      break;
   case EDefaultIconType::kColorTemperatureValue:
      iconPath = pluginPath + EDefaultIconType::kColorTemperature.toString();
      break;
   case EDefaultIconType::kContrastValue:
      iconPath = pluginPath + EDefaultIconType::kContrast.toString();
      break;
   case EDefaultIconType::kBroadcastOnValue:
      iconPath = pluginPath + EDefaultIconType::kBroadcastOn.toString();
      break;
   case EDefaultIconType::kBroadcastOffValue:
      iconPath = pluginPath + EDefaultIconType::kBroadcastOff.toString();
      break;
   case EDefaultIconType::kLightOffValue:
      iconPath = pluginPath + EDefaultIconType::kLightOff.toString();
      break;
   case EDefaultIconType::kLightColorValue:
      iconPath = pluginPath + EDefaultIconType::kLightColor.toString();
      break;
   case EDefaultIconType::kPartyPopperValue:
      iconPath = pluginPath + EDefaultIconType::kPartyPopper.toString();
      break;
   case EDefaultIconType::kSystemBackValue:
      iconPath = pluginPath + EDefaultIconType::kSystemBack.toString();
      break;

   default:
      throw std::runtime_error("getIconPath : icon path does not exist");
   }
   return iconPath;
}
