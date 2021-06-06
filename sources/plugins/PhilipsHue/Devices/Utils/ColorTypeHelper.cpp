#include "ColorTypeHelper.h"
#include "HueLightsResponseType.h"

const std::set<std::string> CColorTypeHelper::GamutBTypes{"LCT001", "LCT002", "LCT003", "LCT007", "LLM001"};
const std::set<std::string> CColorTypeHelper::GamutCTypes{
   "LCT010", "LCT011", "LCT012", "LCT014", "LCT015", "LCT016", "LLC020", "LST002", "LCA003", "LCB001"
};
const std::set<std::string> CColorTypeHelper::GamutATypes{
   "LST001", "LLC005", "LLC006", "LLC007", "LLC010", "LLC011", "LLC012", "LLC013", "LLC014"
};


EColorType CColorTypeHelper::getColorType(boost::shared_ptr<shared::CDataContainer>& response, std::string& type,
                                          int& lightCounter)
{
   boost::algorithm::to_lower(type);
   if (type == ELightType::kON_OFF_LIGHT.toString() || type == ELightType::kON_OFF_PLUGIN_IN_UNIT.toString())
   {
      return EColorType::kNONE;
   }
   if (type == ELightType::kDIMMABLE_LIGHT.toString() || type == ELightType::kDIMMABLE_PLUGIN_IN_UNIT.toString())
   {
      return EColorType::kNONE;
   }
   if (type == ELightType::kCOLOR_TEMPERATURE_LIGHT.toString())
   {
      return EColorType::kTEMPERATURE;
   }
   if (type == ELightType::kCOLOR_LIGHT.toString())
   {
      return getGamutOrLegacyColorType(response, false, lightCounter);
   }
   if (type == ELightType::kEXTENDED_COLOR_LIGHT.toString())
   {
      return getGamutOrLegacyColorType(response, true, lightCounter);
   }

   const auto message = "Could not determine Light color type";
   YADOMS_LOG(error) << message;
   throw std::runtime_error(message);
}

ELightType CColorTypeHelper::getLightType(std::string& type)
{
   boost::algorithm::to_lower(type);
   if (type == ELightType::kON_OFF_LIGHT.toString())
   {
      return ELightType::kON_OFF_LIGHT;
   }
   if (type == ELightType::kON_OFF_PLUGIN_IN_UNIT.toString())
   {
      return ELightType::kON_OFF_PLUGIN_IN_UNIT;
   }
   if (type == ELightType::kDIMMABLE_LIGHT.toString())
   {
      return ELightType::kDIMMABLE_LIGHT;
   }
   if (type == ELightType::kDIMMABLE_PLUGIN_IN_UNIT.toString())
   {
      return ELightType::kDIMMABLE_PLUGIN_IN_UNIT;
   }
   if (type == ELightType::kCOLOR_TEMPERATURE_LIGHT.toString())
   {
      return ELightType::kCOLOR_TEMPERATURE_LIGHT;
   }
   if (type == ELightType::kCOLOR_LIGHT.toString())
   {
      return ELightType::kCOLOR_LIGHT;
   }
   if (type == ELightType::kEXTENDED_COLOR_LIGHT.toString())
   {
      return ELightType::kEXTENDED_COLOR_LIGHT;
   }

   const auto message = "Could not determine Light type";
   YADOMS_LOG(error) << message;
   throw std::runtime_error(message);
}

EColorType CColorTypeHelper::getGamutOrLegacyColorType(boost::shared_ptr<shared::CDataContainer>& response, bool hasCt,
                                                       int& lightCounter)
{
   // Try to get color Gamut from capabilities
   if (response->exists(
         std::to_string(lightCounter) + "." + EHueLightResponseType::kCapabilitiesControlColorGamutType.toString()) && !
      response
      ->
      isNull(std::to_string(lightCounter) + "." + EHueLightResponseType::kCapabilitiesControlColorGamutType.toString()))
   {
      const auto gamut = response->get<std::string>(std::to_string(lightCounter) + "." +
         EHueLightResponseType::kCapabilitiesControlColorGamutType.toString());
      if (gamut == "A")
      {
         return hasCt ? EColorType::kGAMUT_A_TEMPERATURE : EColorType::kGAMUT_A;
      }
      if (gamut == "B")
      {
         return hasCt ? EColorType::kGAMUT_B_TEMPERATURE : EColorType::kGAMUT_B;
      }
      if (gamut == "C")
      {
         return hasCt ? EColorType::kGAMUT_C_TEMPERATURE : EColorType::kGAMUT_C;
      }
      return hasCt ? EColorType::kGAMUT_OTHER_TEMPERATURE : EColorType::kGAMUT_OTHER;
   }
   // Old version without capabilities, fall back to hardcoded types
   if (response->exists(std::to_string(lightCounter) + "." + EHueLightResponseType::kModelId.toString()) && !response
      ->
      isNull(std::to_string(lightCounter) + "." + EHueLightResponseType::kModelId.toString()))
   {
      const auto modelId = response->get<std::string>(
         std::to_string(lightCounter) + "." + EHueLightResponseType::kModelId.toString());

      if (GamutATypes.count(modelId))
      {
         return hasCt ? EColorType::kGAMUT_A_TEMPERATURE : EColorType::kGAMUT_A;
      }
      if (GamutBTypes.count(modelId))
      {
         return hasCt ? EColorType::kGAMUT_B_TEMPERATURE : EColorType::kGAMUT_B;
      }
      if (GamutCTypes.count(modelId))
      {
         return hasCt ? EColorType::kGAMUT_C_TEMPERATURE : EColorType::kGAMUT_C;
      }
      return hasCt ? EColorType::kGAMUT_OTHER_TEMPERATURE : EColorType::kGAMUT_OTHER;
   }
   return EColorType::kUNDEFINED;
}
