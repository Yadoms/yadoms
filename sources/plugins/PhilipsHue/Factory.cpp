#include "Factory.h"
#include "HueService.h"
#include "HueBridgeDiscovery.h"
#include "Devices/Interfaces/ILightsService.h"
#include "Devices/Lights/Service//LightsService.h"
#include "Devices/Utils/LightType.h"
#include "Devices/Lights/Models/LCT015.h"

boost::shared_ptr<IHueService> CFactory::createHueService(shared::event::CEventHandler& mainEventHandler,
                                                          int evtKeyStateReceived,
                                                          int evtKeyStateTimeout,
                                                          boost::shared_ptr<CUrlManager>& urlManager)
{
   return boost::make_shared<CHueService>(mainEventHandler,
                                          evtKeyStateReceived,
                                          evtKeyStateTimeout,
                                          urlManager);
}

boost::shared_ptr<IHueBridgeDiscovery> CFactory::createHueBridgeDiscovery(boost::shared_ptr<CUrlManager>& urlManager)
{
   return boost::make_shared<CHueBridgeDiscovery>(urlManager);
}

boost::shared_ptr<IHueBridgeDiscovery> CFactory::createHueBridgeDiscovery()
{
   return boost::make_shared<CHueBridgeDiscovery>();
}

boost::shared_ptr<ILightsService> CFactory::createLightsService(boost::shared_ptr<CUrlManager>& urlManager)
{
   return boost::make_shared<CLightsService>(urlManager);
}

boost::shared_ptr<ILight> CFactory::createLight(boost::shared_ptr<CUrlManager>& urlManager,
                                                std::pair<int, CHueLightInformations>& lightInformations)
{
   boost::shared_ptr<ILight> light = nullptr;
   switch (lightInformations.second.getModelId())
   {
   case ELightType::kLCT015Value:
      light = boost::make_shared<CLct015>(urlManager, lightInformations);
      break;
   default:
      break;
   }
   return light;
}
