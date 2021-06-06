#include "Factory.h"
#include "HueService.h"
#include "HueBridgeDiscovery.h"
#include "Devices/Interfaces/ILightsService.h"
#include "Devices/Lights/Service//LightsService.h"
#include "Devices/Lights/Models/GamutC.h"
#include "Devices/Lights/Models/GamutA.h"
#include "Devices/Lights/Models/GamutB.h"
#include "Devices/Lights/Models/GamutOther.h"
#include "Devices/Lights/Models/GamutNone.h"

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
   switch (lightInformations.second.getColorType())
   {
   case EColorType::kGAMUT_AValue:
   case EColorType::kGAMUT_A_TEMPERATUREValue:
      light = boost::make_shared<CGamutA>(urlManager, lightInformations);
      break;
   case EColorType::kGAMUT_BValue:
   case EColorType::kGAMUT_B_TEMPERATUREValue:
      light = boost::make_shared<CGamutB>(urlManager, lightInformations);
      break;
   case EColorType::kGAMUT_CValue:
   case EColorType::kGAMUT_C_TEMPERATUREValue:
      light = boost::make_shared<CGamutC>(urlManager, lightInformations);
      break;
   case EColorType::kGAMUT_OTHERValue:
   case EColorType::kGAMUT_OTHER_TEMPERATUREValue:
      light = boost::make_shared<CGamutOther>(urlManager, lightInformations);
      break;
   default:
      light = boost::make_shared<CGamutNone>(urlManager, lightInformations);
      break;
   }
   return light;
}
