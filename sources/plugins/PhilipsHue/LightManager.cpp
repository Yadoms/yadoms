#include "LightManager.h"
#include "shared/Log.h"
#include "shared/http/HttpMethods.h"
#include "ColorConverter.h"
#include "Entities/HueStreaming.h"

CLightManager::CLightManager(boost::shared_ptr<CUrlManager>& urlManager)
   : m_urlManager(urlManager),
     m_lightId(0)
{
}

std::map<int, CHueLightInformations> CLightManager::getAllLights()
{
   std::map<int, CHueLightInformations> hueLightsInformations;
   const auto urlPatternPath = m_urlManager->getUrlPatternPath(CUrlManager::kGetAllLights);
   const auto lightUrl = m_urlManager->getPatternUrl(urlPatternPath);

   try
   {
      const auto response = shared::http::CHttpMethods::sendJsonGetRequest(lightUrl);
      auto lightCounter = 1;;
      while (response->exists(std::to_string(lightCounter)))
      {
         CHueLightInformations hueLightInformations;
         CHueState state;
         // Fill State
         if (response->exists(std::to_string(lightCounter) + "." + EHueLightResponseType::kStateOn.toString()) && !
            response
            ->
            isNull(std::to_string(lightCounter) + "." + EHueLightResponseType::kStateOn.toString()))
         {
            state.setOn(
               response->get<bool>(std::to_string(lightCounter) + "." + EHueLightResponseType::kStateOn.toString()));
         }
         if (response->exists(std::to_string(lightCounter) + "." + EHueLightResponseType::kStateBri.toString()) && !
            response
            ->
            isNull(std::to_string(lightCounter) + "." + EHueLightResponseType::kStateBri.toString()))
         {
            state.setBri(
               response->get<int>(std::to_string(lightCounter) + "." + EHueLightResponseType::kStateBri.toString()));
         }
         if (response->exists(std::to_string(lightCounter) + "." + EHueLightResponseType::kStateHue.toString()) && !
            response
            ->
            isNull(std::to_string(lightCounter) + "." + EHueLightResponseType::kStateHue.toString()))
         {
            state.setHue(
               response->get<int>(std::to_string(lightCounter) + "." + EHueLightResponseType::kStateHue.toString()));
         }
         if (response->exists(std::to_string(lightCounter) + "." + EHueLightResponseType::kStateSat.toString()) && !
            response
            ->
            isNull(std::to_string(lightCounter) + "." + EHueLightResponseType::kStateSat.toString()))
         {
            state.setSat(
               response->get<int>(std::to_string(lightCounter) + "." + EHueLightResponseType::kStateSat.toString()));
         }
         if (response->exists(std::to_string(lightCounter) + "." + EHueLightResponseType::kStateEffect.toString()) && !
            response
            ->
            isNull(std::to_string(lightCounter) + "." + EHueLightResponseType::kStateEffect.toString()))
         {
            state.setEffect(
               response->get<std::string>(
                  std::to_string(lightCounter) + "." + EHueLightResponseType::kStateEffect.toString()));
         }
         // Fill Xy
         CXy xy;
         if (response->exists(std::to_string(lightCounter) + "." + EHueLightResponseType::kStateXyX.toString()) && !
            response
            ->
            isNull(std::to_string(lightCounter) + "." + EHueLightResponseType::kStateXyX.toString()))
         {
            xy.setX(response->get<float>(
               std::to_string(lightCounter) + "." + EHueLightResponseType::kStateXyX.toString()));
         }
         if (response->exists(std::to_string(lightCounter) + "." + EHueLightResponseType::kStateXyY.toString()) && !
            response
            ->
            isNull(std::to_string(lightCounter) + "." + EHueLightResponseType::kStateXyY.toString()))
         {
            xy.setY(response->get<float>(
               std::to_string(lightCounter) + "." + EHueLightResponseType::kStateXyY.toString()));
         }
         state.setXy(xy);

         if (response->exists(std::to_string(lightCounter) + "." + EHueLightResponseType::kStateCt.toString()) && !
            response
            ->
            isNull(std::to_string(lightCounter) + "." + EHueLightResponseType::kStateCt.toString()))
         {
            state.setCt(
               response->get<int>(std::to_string(lightCounter) + "." + EHueLightResponseType::kStateCt.toString()));
         }

         if (response->exists(std::to_string(lightCounter) + "." + EHueLightResponseType::kStateAlert.toString()) && !
            response
            ->
            isNull(std::to_string(lightCounter) + "." + EHueLightResponseType::kStateAlert.toString()))
         {
            state.setAlert(
               response->get<std::string>(
                  std::to_string(lightCounter) + "." + EHueLightResponseType::kStateAlert.toString()));
         }

         if (response->exists(std::to_string(lightCounter) + "." + EHueLightResponseType::kStateColorMode.toString()) &&
            !response
            ->
            isNull(std::to_string(lightCounter) + "." + EHueLightResponseType::kStateColorMode.toString()))
         {
            state.setColorMode(response->get<std::string>(
               std::to_string(lightCounter) + "." + EHueLightResponseType::kStateColorMode.toString()));
         }

         if (response->exists(std::to_string(lightCounter) + "." + EHueLightResponseType::kStateMode.toString()) && !
            response
            ->
            isNull(std::to_string(lightCounter) + "." + EHueLightResponseType::kStateMode.toString()))
         {
            state.setMode(
               response->get<std::string>(
                  std::to_string(lightCounter) + "." + EHueLightResponseType::kStateMode.toString()));
         }

         if (response->exists(std::to_string(lightCounter) + "." + EHueLightResponseType::kStateReachable.toString()) &&
            !response
            ->
            isNull(std::to_string(lightCounter) + "." + EHueLightResponseType::kStateReachable.toString()))
         {
            state.setReachable(
               response->get<bool>(
                  std::to_string(lightCounter) + "." + EHueLightResponseType::kStateReachable.toString()));
         }

         hueLightInformations.setState(state);

         // Fill SwUpdate
         CHueSwUpdate swUpdate;
         if (response->exists(std::to_string(lightCounter) + "." + EHueLightResponseType::kSwUpdateState.toString()) &&
            !response
            ->
            isNull(std::to_string(lightCounter) + "." + EHueLightResponseType::kSwUpdateState.toString()))
         {
            swUpdate.setState(response->get<std::string>(
               std::to_string(lightCounter) + "." + EHueLightResponseType::kSwUpdateState.toString()));
         }
         if (response->exists(
               std::to_string(lightCounter) + "." + EHueLightResponseType::kSwUpdateLastInstall.toString()) && !response
            ->
            isNull(std::to_string(lightCounter) + "." + EHueLightResponseType::kSwUpdateLastInstall.toString()))
         {
            swUpdate.setLastInstall(response->get<std::string>(
               std::to_string(lightCounter) + "." + EHueLightResponseType::kSwUpdateLastInstall.toString()));
         }

         hueLightInformations.setSwUpdate(swUpdate);

         // Fill main hueLightInformations
         if (response->exists(std::to_string(lightCounter) + "." + EHueLightResponseType::kType.toString()) && !response
            ->
            isNull(std::to_string(lightCounter) + "." + EHueLightResponseType::kType.toString()))
         {
            hueLightInformations.setType(
               response->get<std::string>(
                  std::to_string(lightCounter) + "." + EHueLightResponseType::kType.toString()));
         }
         if (response->exists(std::to_string(lightCounter) + "." + EHueLightResponseType::kName.toString()) && !response
            ->
            isNull(std::to_string(lightCounter) + "." + EHueLightResponseType::kName.toString()))
         {
            hueLightInformations.setName(
               response->get<std::string>(
                  std::to_string(lightCounter) + "." + EHueLightResponseType::kName.toString()));
         }

         if (response->exists(std::to_string(lightCounter) + "." + EHueLightResponseType::kModelId.toString()) && !
            response
            ->
            isNull(std::to_string(lightCounter) + "." + EHueLightResponseType::kModelId.toString()))
         {
            hueLightInformations.setModelId(
               response->get<std::string>(
                  std::to_string(lightCounter) + "." + EHueLightResponseType::kModelId.toString()));
         }

         if (response->exists(std::to_string(lightCounter) + "." + EHueLightResponseType::kManufacturerName.toString())
            && !response
            ->
            isNull(std::to_string(lightCounter) + "." + EHueLightResponseType::kManufacturerName.toString()))
         {
            hueLightInformations.setManufacturerName(response->get<std::string>(
               std::to_string(lightCounter) + "." + EHueLightResponseType::kManufacturerName.toString()));
         }

         if (response->exists(std::to_string(lightCounter) + "." + EHueLightResponseType::kProductName.toString()) && !
            response
            ->
            isNull(std::to_string(lightCounter) + "." + EHueLightResponseType::kProductName.toString()))
         {
            hueLightInformations.setProductName(
               response->get<std::string>(
                  std::to_string(lightCounter) + "." + EHueLightResponseType::kProductName.toString()));
         }

         // Fill capabilities
         CHueCapabilities capabilities;
         if (response->exists(
               std::to_string(lightCounter) + "." + EHueLightResponseType::kCapabilitiesCertified.toString()) && !
            response
            ->
            isNull(std::to_string(lightCounter) + "." + EHueLightResponseType::kCapabilitiesCertified.toString()))
         {
            capabilities.setCertified(response->get<bool>(
               std::to_string(lightCounter) + "." + EHueLightResponseType::kCapabilitiesCertified.toString()));
         }
         CHueControl control;
         if (response->exists(
               std::to_string(lightCounter) + "." + EHueLightResponseType::kCapabilitiesControlMinDimLevel.toString())
            && !response
            ->
            isNull(
               std::to_string(lightCounter) + "." + EHueLightResponseType::kCapabilitiesControlMinDimLevel.toString()))
         {
            control.setMinDimLevel(response->get<int>(
               std::to_string(lightCounter) + "." + EHueLightResponseType::kCapabilitiesControlMinDimLevel.toString()));
         }

         if (response->exists(
               std::to_string(lightCounter) + "." + EHueLightResponseType::kCapabilitiesControlMaxLumen.toString()) && !
            response
            ->
            isNull(std::to_string(lightCounter) + "." + EHueLightResponseType::kCapabilitiesControlMaxLumen.toString()))
         {
            control.setMaxLumen(response->get<int>(
               std::to_string(lightCounter) + "." + EHueLightResponseType::kCapabilitiesControlMaxLumen.toString()));
         }

         if (response->exists(
               std::to_string(lightCounter) + "." + EHueLightResponseType::kCapabilitiesControlColorGamutType.
               toString()) && !response
            ->
            isNull(
               std::to_string(lightCounter) + "." + EHueLightResponseType::kCapabilitiesControlColorGamutType.
               toString()))
         {
            control.setColorGamutType(response->get<std::string>(
               std::to_string(lightCounter) + "." + EHueLightResponseType::kCapabilitiesControlColorGamutType.
               toString()));
         }
         //TODO : fill COLORGAMUT
         // Fill Ct
         CHueCt ct;
         if (response->exists(
               std::to_string(lightCounter) + "." + EHueLightResponseType::kCapabilitiesControlCtMin.toString()) && !
            response
            ->
            isNull(std::to_string(lightCounter) + "." + EHueLightResponseType::kCapabilitiesControlCtMin.toString()))
         {
            ct.setMin(response->get<int>(
               std::to_string(lightCounter) + "." + EHueLightResponseType::kCapabilitiesControlCtMin.toString()));
         }
         if (response->exists(
               std::to_string(lightCounter) + "." + EHueLightResponseType::kCapabilitiesControlCtMax.toString()) && !
            response
            ->
            isNull(std::to_string(lightCounter) + "." + EHueLightResponseType::kCapabilitiesControlCtMax.toString()))
         {
            ct.setMax(response->get<int>(
               std::to_string(lightCounter) + "." + EHueLightResponseType::kCapabilitiesControlCtMax.toString()));
         }
         control.setCt(ct);
         capabilities.setControl(control);

         // Fill streaming
         CHueStreaming streaming;
         if (response->exists(
               std::to_string(lightCounter) + "." + EHueLightResponseType::kCapabilitiesStreamingRenderer.toString()) &&
            !response
            ->
            isNull(
               std::to_string(lightCounter) + "." + EHueLightResponseType::kCapabilitiesStreamingRenderer.toString()))
         {
            streaming.setRenderer(response->get<bool>(
               std::to_string(lightCounter) + "." + EHueLightResponseType::kCapabilitiesStreamingRenderer.toString()));
         }
         if (response->exists(
               std::to_string(lightCounter) + "." + EHueLightResponseType::kCapabilitiesStreamingProxy.toString()) && !
            response
            ->
            isNull(std::to_string(lightCounter) + "." + EHueLightResponseType::kCapabilitiesStreamingProxy.toString()))
         {
            streaming.setProxy(response->get<bool>(
               std::to_string(lightCounter) + "." + EHueLightResponseType::kCapabilitiesStreamingProxy.toString()));
         }
         capabilities.setStreaming(streaming);

         hueLightInformations.setCapabilities(capabilities);

         // Fill config
         CHueConfig config;
         if (response->exists(std::to_string(lightCounter) + "." + EHueLightResponseType::kConfigArchetype.toString())
            && !response
            ->
            isNull(std::to_string(lightCounter) + "." + EHueLightResponseType::kConfigArchetype.toString()))
         {
            config.setArchetype(response->get<std::string>(
               std::to_string(lightCounter) + "." + EHueLightResponseType::kConfigArchetype.toString()));
         }

         if (response->exists(std::to_string(lightCounter) + "." + EHueLightResponseType::kConfigFunction.toString()) &&
            !response
            ->
            isNull(std::to_string(lightCounter) + "." + EHueLightResponseType::kConfigFunction.toString()))
         {
            config.setFunction(response->get<std::string>(
               std::to_string(lightCounter) + "." + EHueLightResponseType::kConfigFunction.toString()));
         }

         if (response->exists(std::to_string(lightCounter) + "." + EHueLightResponseType::kConfigDirection.toString())
            && !response
            ->
            isNull(std::to_string(lightCounter) + "." + EHueLightResponseType::kConfigDirection.toString()))
         {
            config.setDirection(response->get<std::string>(
               std::to_string(lightCounter) + "." + EHueLightResponseType::kConfigDirection.toString()));
         }
         CHueStartUp startUp;
         if (response->exists(
               std::to_string(lightCounter) + "." + EHueLightResponseType::kConfigStartUpMode.toString()) && !response
            ->
            isNull(std::to_string(lightCounter) + "." + EHueLightResponseType::kConfigStartUpMode.toString()))
         {
            startUp.setMode(response->get<std::string>(
               std::to_string(lightCounter) + "." + EHueLightResponseType::kConfigStartUpMode.toString()));
         }

         if (response->exists(
               std::to_string(lightCounter) + "." + EHueLightResponseType::kConfigStartUpConfigured.toString()) && !
            response
            ->
            isNull(std::to_string(lightCounter) + "." + EHueLightResponseType::kConfigStartUpConfigured.toString()))
         {
            startUp.setConfigured(response->get<bool>(
               std::to_string(lightCounter) + "." + EHueLightResponseType::kConfigStartUpConfigured.toString()));
         }
         config.setStartUp(startUp);

         hueLightInformations.setConfig(config);

         // Fill main hueLightInformations
         if (response->exists(std::to_string(lightCounter) + "." + EHueLightResponseType::kUniqueId.toString()) && !
            response
            ->
            isNull(std::to_string(lightCounter) + "." + EHueLightResponseType::kUniqueId.toString()))
         {
            hueLightInformations.
               setUniqueId(
                  response->get<std::string>(
                     std::to_string(lightCounter) + "." + EHueLightResponseType::kUniqueId.toString()));
         }

         if (response->exists(std::to_string(lightCounter) + "." + EHueLightResponseType::kSwVersion.toString()) && !
            response
            ->
            isNull(std::to_string(lightCounter) + "." + EHueLightResponseType::kSwVersion.toString()))
         {
            hueLightInformations.setSwVersion(
               response->get<std::string>(
                  std::to_string(lightCounter) + "." + EHueLightResponseType::kSwVersion.toString()));
         }

         if (response->exists(std::to_string(lightCounter) + "." + EHueLightResponseType::kSwConfigId.toString()) && !
            response
            ->
            isNull(std::to_string(lightCounter) + "." + EHueLightResponseType::kSwConfigId.toString()))
         {
            hueLightInformations.setSwConfigId(
               response->get<std::string>(
                  std::to_string(lightCounter) + "." + EHueLightResponseType::kSwConfigId.toString()));
         }

         if (response->exists(std::to_string(lightCounter) + "." + EHueLightResponseType::kProductId.toString()) && !
            response
            ->
            isNull(std::to_string(lightCounter) + "." + EHueLightResponseType::kProductId.toString()))
         {
            hueLightInformations.setProductId(
               response->get<std::string>(
                  std::to_string(lightCounter) + "." + EHueLightResponseType::kProductId.toString()));
         }

         hueLightsInformations.insert({lightCounter, hueLightInformations});
         lightCounter++;
      }
   }
   catch (std::exception& e)
   {
      const auto message = (boost::format("Fail to send Get http request or interpret answer \"%1%\" : %2%") % lightUrl
         %
         e.what()).str();
      YADOMS_LOG(error) << "Fail to send Get http request or interpret answer " << lightUrl << " : " << e.what();
      throw;
   }
   return hueLightsInformations;
}

CHueLightInformations CLightManager::getLightAttributesAndState(const int id)
{
   CHueLightInformations hueLightAttributesAndState;
   const auto urlPatternPath = m_urlManager->getUrlPatternPath(CUrlManager::kGetLightAttributesAndState, id);
   const auto lightUrl = m_urlManager->getPatternUrl(urlPatternPath);

   try
   {
      auto response = shared::http::CHttpMethods::sendJsonGetRequest(lightUrl);

      hueLightAttributesAndState.setState(getHueLightInformationsState(response));
      hueLightAttributesAndState.setSwUpdate(getHueLightInformationsSwUpdate(response));

      if (response->exists(EHueLightResponseType::kType.toString()) && !response
         ->
         isNull(EHueLightResponseType::kType.toString()))
      {
         hueLightAttributesAndState.setType(response->get<std::string>(EHueLightResponseType::kType.toString()));
      }
      if (response->exists(EHueLightResponseType::kName.toString()) && !response
         ->
         isNull(EHueLightResponseType::kName.toString()))
      {
         hueLightAttributesAndState.setName(response->get<std::string>(EHueLightResponseType::kName.toString()));
      }

      if (response->exists(EHueLightResponseType::kModelId.toString()) && !response
         ->
         isNull(EHueLightResponseType::kModelId.toString()))
      {
         hueLightAttributesAndState.setModelId(response->get<std::string>(EHueLightResponseType::kModelId.toString()));
      }

      if (response->exists(EHueLightResponseType::kManufacturerName.toString()) && !response
         ->
         isNull(EHueLightResponseType::kManufacturerName.toString()))
      {
         hueLightAttributesAndState.setManufacturerName(
            response->get<std::string>(EHueLightResponseType::kManufacturerName.toString()));
      }

      if (response->exists(EHueLightResponseType::kProductName.toString()) && !response
         ->
         isNull(EHueLightResponseType::kProductName.toString()))
      {
         hueLightAttributesAndState.setProductName(
            response->get<std::string>(EHueLightResponseType::kProductName.toString()));
      }

      hueLightAttributesAndState.setCapabilities(getHueLightInformationsCapabilities(response));
      hueLightAttributesAndState.setConfig(getHueLightInformationsConfig(response));

      if (response->exists(EHueLightResponseType::kUniqueId.toString()) && !response
         ->
         isNull(EHueLightResponseType::kUniqueId.toString()))
      {
         hueLightAttributesAndState.
            setUniqueId(response->get<std::string>(EHueLightResponseType::kUniqueId.toString()));
      }

      if (response->exists(EHueLightResponseType::kSwVersion.toString()) && !response
         ->
         isNull(EHueLightResponseType::kSwVersion.toString()))
      {
         hueLightAttributesAndState.setSwVersion(
            response->get<std::string>(EHueLightResponseType::kSwVersion.toString()));
      }

      if (response->exists(EHueLightResponseType::kSwConfigId.toString()) && !response
         ->
         isNull(EHueLightResponseType::kSwConfigId.toString()))
      {
         hueLightAttributesAndState.setSwConfigId(
            response->get<std::string>(EHueLightResponseType::kSwConfigId.toString()));
      }

      if (response->exists(EHueLightResponseType::kProductId.toString()) && !response
         ->
         isNull(EHueLightResponseType::kProductId.toString()))
      {
         hueLightAttributesAndState.setProductId(
            response->get<std::string>(EHueLightResponseType::kProductId.toString()));
      }
   }
   catch (std::exception& e)
   {
      const auto message = (boost::format("Fail to send Get http request or interpret answer \"%1%\" : %2%") % lightUrl
         %
         e.what()).str();
      YADOMS_LOG(error) << "Fail to send Get http request or interpret answer " << lightUrl << " : " << e.what();
      throw;
   }
   return hueLightAttributesAndState;
}

CHueState CLightManager::getHueLightInformationsState(boost::shared_ptr<shared::CDataContainer>& response)
{
   CHueState state;
   if (response->exists(EHueLightResponseType::kStateOn.toString()) && !response
      ->
      isNull(EHueLightResponseType::kStateOn.toString()))
   {
      state.setOn(response->get<bool>(EHueLightResponseType::kStateOn.toString()));
   }
   if (response->exists(EHueLightResponseType::kStateBri.toString()) && !response
      ->
      isNull(EHueLightResponseType::kStateBri.toString()))
   {
      state.setBri(response->get<int>(EHueLightResponseType::kStateBri.toString()));
   }
   if (response->exists(EHueLightResponseType::kStateHue.toString()) && !response
      ->
      isNull(EHueLightResponseType::kStateHue.toString()))
   {
      state.setHue(response->get<int>(EHueLightResponseType::kStateHue.toString()));
   }
   if (response->exists(EHueLightResponseType::kStateSat.toString()) && !response
      ->
      isNull(EHueLightResponseType::kStateSat.toString()))
   {
      state.setSat(response->get<int>(EHueLightResponseType::kStateSat.toString()));
   }
   if (response->exists(EHueLightResponseType::kStateEffect.toString()) && !response
      ->
      isNull(EHueLightResponseType::kStateEffect.toString()))
   {
      state.setEffect(response->get<std::string>(EHueLightResponseType::kStateEffect.toString()));
   }
   CXy xy;
   if (response->exists(EHueLightResponseType::kStateXyX.toString()) && !response
      ->
      isNull(EHueLightResponseType::kStateXyX.toString()))
   {
      xy.setX(response->get<float>(EHueLightResponseType::kStateXyX.toString()));
   }
   if (response->exists(EHueLightResponseType::kStateXyY.toString()) && !response
      ->
      isNull(EHueLightResponseType::kStateXyY.toString()))
   {
      xy.setY(response->get<float>(EHueLightResponseType::kStateXyY.toString()));
   }
   state.setXy(xy);

   if (response->exists(EHueLightResponseType::kStateCt.toString()) && !response
      ->
      isNull(EHueLightResponseType::kStateCt.toString()))
   {
      state.setCt(response->get<int>(EHueLightResponseType::kStateCt.toString()));
   }

   if (response->exists(EHueLightResponseType::kStateAlert.toString()) && !response
      ->
      isNull(EHueLightResponseType::kStateAlert.toString()))
   {
      state.setAlert(response->get<std::string>(EHueLightResponseType::kStateAlert.toString()));
   }

   if (response->exists(EHueLightResponseType::kStateColorMode.toString()) && !response
      ->
      isNull(EHueLightResponseType::kStateColorMode.toString()))
   {
      state.setColorMode(response->get<std::string>(EHueLightResponseType::kStateColorMode.toString()));
   }

   if (response->exists(EHueLightResponseType::kStateMode.toString()) && !response
      ->
      isNull(EHueLightResponseType::kStateMode.toString()))
   {
      state.setMode(response->get<std::string>(EHueLightResponseType::kStateMode.toString()));
   }

   if (response->exists(EHueLightResponseType::kStateReachable.toString()) && !response
      ->
      isNull(EHueLightResponseType::kStateReachable.toString()))
   {
      state.setReachable(response->get<bool>(EHueLightResponseType::kStateReachable.toString()));
   }
   return state;
}

CHueSwUpdate CLightManager::getHueLightInformationsSwUpdate(boost::shared_ptr<shared::CDataContainer>& response)
{
   CHueSwUpdate swUpdate;
   if (response->exists(EHueLightResponseType::kSwUpdateState.toString()) && !response
      ->
      isNull(EHueLightResponseType::kSwUpdateState.toString()))
   {
      swUpdate.setState(response->get<std::string>(EHueLightResponseType::kSwUpdateState.toString()));
   }
   if (response->exists(EHueLightResponseType::kSwUpdateLastInstall.toString()) && !response
      ->
      isNull(EHueLightResponseType::kSwUpdateLastInstall.toString()))
   {
      swUpdate.setLastInstall(response->get<std::string>(EHueLightResponseType::kSwUpdateLastInstall.toString()));
   }
   return swUpdate;
}

CHueCapabilities CLightManager::getHueLightInformationsCapabilities(boost::shared_ptr<shared::CDataContainer>& response)
{
   CHueCapabilities capabilities;
   if (response->exists(EHueLightResponseType::kCapabilitiesCertified.toString()) && !response
      ->
      isNull(EHueLightResponseType::kCapabilitiesCertified.toString()))
   {
      capabilities.setCertified(response->get<bool>(EHueLightResponseType::kCapabilitiesCertified.toString()));
   }
   CHueControl control;
   if (response->exists(EHueLightResponseType::kCapabilitiesControlMinDimLevel.toString()) && !response
      ->
      isNull(EHueLightResponseType::kCapabilitiesControlMinDimLevel.toString()))
   {
      control.setMinDimLevel(response->get<int>(EHueLightResponseType::kCapabilitiesControlMinDimLevel.toString()));
   }

   if (response->exists(EHueLightResponseType::kCapabilitiesControlMaxLumen.toString()) && !response
      ->
      isNull(EHueLightResponseType::kCapabilitiesControlMaxLumen.toString()))
   {
      control.setMaxLumen(response->get<int>(EHueLightResponseType::kCapabilitiesControlMaxLumen.toString()));
   }

   if (response->exists(EHueLightResponseType::kCapabilitiesControlColorGamutType.toString()) && !response
      ->
      isNull(EHueLightResponseType::kCapabilitiesControlColorGamutType.toString()))
   {
      control.setColorGamutType(
         response->get<std::string>(EHueLightResponseType::kCapabilitiesControlColorGamutType.toString()));
   }
   //TODO : fill COLORGAMUT
   CHueCt ct;
   if (response->exists(EHueLightResponseType::kCapabilitiesControlCtMin.toString()) && !response
      ->
      isNull(EHueLightResponseType::kCapabilitiesControlCtMin.toString()))
   {
      ct.setMin(response->get<int>(EHueLightResponseType::kCapabilitiesControlCtMin.toString()));
   }
   if (response->exists(EHueLightResponseType::kCapabilitiesControlCtMax.toString()) && !response
      ->
      isNull(EHueLightResponseType::kCapabilitiesControlCtMax.toString()))
   {
      ct.setMax(response->get<int>(EHueLightResponseType::kCapabilitiesControlCtMax.toString()));
   }
   control.setCt(ct);
   capabilities.setControl(control);

   CHueStreaming streaming;
   if (response->exists(EHueLightResponseType::kCapabilitiesStreamingRenderer.toString()) && !response
      ->
      isNull(EHueLightResponseType::kCapabilitiesStreamingRenderer.toString()))
   {
      streaming.setRenderer(response->get<bool>(EHueLightResponseType::kCapabilitiesStreamingRenderer.toString()));
   }
   if (response->exists(EHueLightResponseType::kCapabilitiesStreamingProxy.toString()) && !response
      ->
      isNull(EHueLightResponseType::kCapabilitiesStreamingProxy.toString()))
   {
      streaming.setProxy(response->get<bool>(EHueLightResponseType::kCapabilitiesStreamingProxy.toString()));
   }
   capabilities.setStreaming(streaming);
   return capabilities;
}

CHueConfig CLightManager::getHueLightInformationsConfig(boost::shared_ptr<shared::CDataContainer>& response)
{
   CHueConfig config;
   if (response->exists(EHueLightResponseType::kConfigArchetype.toString()) && !response
      ->
      isNull(EHueLightResponseType::kConfigArchetype.toString()))
   {
      config.setArchetype(response->get<std::string>(EHueLightResponseType::kConfigArchetype.toString()));
   }

   if (response->exists(EHueLightResponseType::kConfigFunction.toString()) && !response
      ->
      isNull(EHueLightResponseType::kConfigFunction.toString()))
   {
      config.setFunction(response->get<std::string>(EHueLightResponseType::kConfigFunction.toString()));
   }

   if (response->exists(EHueLightResponseType::kConfigDirection.toString()) && !response
      ->
      isNull(EHueLightResponseType::kConfigDirection.toString()))
   {
      config.setDirection(response->get<std::string>(EHueLightResponseType::kConfigDirection.toString()));
   }
   CHueStartUp startUp;
   if (response->exists(EHueLightResponseType::kConfigStartUpMode.toString()) && !response
      ->
      isNull(EHueLightResponseType::kConfigStartUpMode.toString()))
   {
      startUp.setMode(response->get<std::string>(EHueLightResponseType::kConfigStartUpMode.toString()));
   }

   if (response->exists(EHueLightResponseType::kConfigStartUpConfigured.toString()) && !response
      ->
      isNull(EHueLightResponseType::kConfigStartUpConfigured.toString()))
   {
      startUp.setConfigured(response->get<bool>(EHueLightResponseType::kConfigStartUpConfigured.toString()));
   }
   config.setStartUp(startUp);
   return config;
}

void CLightManager::setLightState(const std::string& lightUrl, shared::CDataContainer& body)
{
   try
   {
      const auto response = shared::http::CHttpMethods::sendJsonPutRequest(lightUrl, body.serialize());
   }
   catch (std::exception& e)
   {
      const auto message = (boost::format("Fail to send Get http request or interpret answer \"%1%\" : %2%") % lightUrl
         %
         e.what()).str();
      YADOMS_LOG(error) << "Fail to send Get http request or interpret answer " << lightUrl << " : " << e.what();
      throw;
   }
}

void CLightManager::setNewLights(const boost::system::error_code& errorCode)
{
   // boost::asio::error::operation_aborted : Normal behaviour-Invoked when we cancelled the timer
   if (errorCode && errorCode != boost::asio::error::operation_aborted)
      throw std::runtime_error("Error trying to get new lights");
   m_newLights = getAllLights();

   closeReadingNewLights();
}

void CLightManager::setLightId(std::string& lightName, std::map<int, CHueLightInformations>& detectedLights)
{
   const auto it = std::find_if(std::begin(detectedLights), std::end(detectedLights),
                                [&lightName](auto&& pair)
                                {
                                   return pair.second.getName() == lightName;
                                });

   if (it == std::end(detectedLights))
   {
      YADOMS_LOG(warning) << "Light not found";
      throw std::runtime_error("Light ID is not found");
   }
   YADOMS_LOG(information) << "Light ID = " << m_lightId << " is found ";
   m_lightId = it->first;
}

void CLightManager::lightOn()
{
   const auto urlPatternPath = m_urlManager->getUrlPatternPath(CUrlManager::kLightState, m_lightId);
   const auto lightUrl = m_urlManager->getPatternUrl(urlPatternPath);

   shared::CDataContainer body;
   body.set("on", true);
   setLightState(lightUrl, body);
}


void CLightManager::lightOff()
{
   const auto urlPatternPath = m_urlManager->getUrlPatternPath(CUrlManager::kLightState, m_lightId);
   const auto lightUrl = m_urlManager->getPatternUrl(urlPatternPath);

   shared::CDataContainer body;
   body.set("on", false);
   setLightState(lightUrl, body);
}

void CLightManager::setLightColorUsingXy(const std::string& hexRgb)
{
   const auto urlPatternPath = m_urlManager->getUrlPatternPath(CUrlManager::kLightState, m_lightId);
   const auto lightUrl = m_urlManager->getPatternUrl(urlPatternPath);

   auto rgb = CColorConverter::hexToRgb(hexRgb);
   const auto xy = CColorConverter::rgbToXy(rgb);

   shared::CDataContainer body;
   body.set("on", true);
   body.set("xy.0", xy.getX());
   body.set("xy.1", xy.getY());

   setLightState(lightUrl, body);
}

void CLightManager::searchForNewLights()
{
   const auto urlPatternPath = m_urlManager->getUrlPatternPath(CUrlManager::kGetAllLights);
   const auto lightUrl = m_urlManager->getPatternUrl(urlPatternPath);
   const std::string body;
   std::map<int, CHueLightInformations> newHueLights;
   try
   {
      const auto response = shared::http::CHttpMethods::sendJsonPostRequest(lightUrl, body);

      startReadingNewLights();
   }
   catch (std::exception& e)
   {
      const auto message = (boost::format("Fail to send Get http request or interpret answer \"%1%\" : %2%") % lightUrl
         %
         e.what()).str();
      YADOMS_LOG(error) << "Fail to send Get http request or interpret answer " << lightUrl << " : " << e.what();
      throw std::runtime_error(message);
   }
}

std::map<int, CHueLightInformations> CLightManager::getNewLights()
{
   return m_newLights;
}

void CLightManager::startReadingNewLights()
{
   boost::asio::steady_timer timer(m_ios, boost::asio::chrono::seconds(40));
   timer.async_wait(boost::bind(&CLightManager::setNewLights, this,
                                boost::asio::placeholders::error));

   m_ios.run();
}

void CLightManager::closeReadingNewLights()
{
   m_ios.stop();
}
