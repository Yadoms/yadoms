#include "ColorTypeHelper.h"
#include "HueLightsResponseType.h"

const std::set<std::string> CColorTypeHelper::GamutBTypes{"LCT001", "LCT002", "LCT003", "LCT007", "LLM001"};
const std::set<std::string> CColorTypeHelper::GamutCTypes{
   "LCT010", "LCT011", "LCT012", "LCT014", "LCT015", "LCT016", "LLC020", "LST002", "LCA003", "LCB001"
};
const std::set<std::string> CColorTypeHelper::GamutATypes{
   "LST001", "LLC005", "LLC006", "LLC007", "LLC010", "LLC011", "LLC012", "LLC013", "LLC014"
};

const std::string CColorTypeHelper::OnOffLight{"on/off light"};
const std::string CColorTypeHelper::OnOffPluginInUnit{"on/off plug-in unit"};
const std::string CColorTypeHelper::DimmableLight{"dimmable light"};
const std::string CColorTypeHelper::DimmablePluginInUnit{"dimmable plug-in unit"};
const std::string CColorTypeHelper::ColorTemperatureLight{"color temperature light"};
const std::string CColorTypeHelper::ColorLight{"color light"};
const std::string CColorTypeHelper::ExtendedColorLight{"extended color light"};

EColorType CColorTypeHelper::getColorType(boost::shared_ptr<shared::CDataContainer>& response)
{
   if (response->exists(EHueLightResponseType::kType.toString()) && !response
      ->
      isNull(EHueLightResponseType::kType.toString()))
   {
      const auto type = response->get<std::string>(EHueLightResponseType::kType.toString());
      if (type == OnOffLight || type == OnOffPluginInUnit)
      {
         return EColorType::kNONE;
      }
      if (type == DimmableLight || type == DimmablePluginInUnit)
      {
         return EColorType::kNONE;
      }
      if (type == ColorTemperatureLight)
      {
         return EColorType::kTEMPERATURE;
      }
      if (type == ColorLight)
      {
         return getGamutOrLegacyColorType(response, false);
      }
      if (type == ExtendedColorLight)
      {
         return getGamutOrLegacyColorType(response, true);
      }
   }
   const auto message = "Could not determine Light type";
   YADOMS_LOG(error) << message;
   throw std::runtime_error(message);
}

EColorType CColorTypeHelper::getGamutOrLegacyColorType(boost::shared_ptr<shared::CDataContainer>& response, bool hasCt)
{
   // Try to get color Gamut from capabilities
   if (response->exists(EHueLightResponseType::kCapabilitiesControlColorGamutType.toString()) && !response
      ->
      isNull(EHueLightResponseType::kCapabilitiesControlColorGamutType.toString()))
   {
      const auto gamut = response->get<std::string>(
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
   if (response->exists(EHueLightResponseType::kModelId.toString()) && !response
      ->
      isNull(EHueLightResponseType::kModelId.toString()))
   {
      const auto modelId = response->get<std::string>(EHueLightResponseType::kModelId.toString());

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
