#pragma once
#include "IHueService.h"
#include "shared/event/EventHandler.hpp"
#include "UrlManager.h"
#include "IHueBridgeDiscovery.h"
#include "Devices/Interfaces/ILightsService.h"
#include "Devices/Interfaces/ILight.h"

class CFactory
{
public:
   CFactory() = default;
   ~CFactory() = default;

   static boost::shared_ptr<IHueService> createHueService(shared::event::CEventHandler& mainEventHandler,
                                                          int evtKeyStateReceived,
                                                          int evtKeyStateTimeout,
                                                          boost::shared_ptr<CUrlManager>& urlManager);

   static boost::shared_ptr<IHueBridgeDiscovery> createHueBridgeDiscovery(boost::shared_ptr<CUrlManager>& urlManager);
   static boost::shared_ptr<IHueBridgeDiscovery> createHueBridgeDiscovery();
   static boost::shared_ptr<ILightsService> createLightsService(boost::shared_ptr<CUrlManager>& urlManager);
   static boost::shared_ptr<ILight> createLight(boost::shared_ptr<CUrlManager>& urlManager,
                                                std::pair<int, CHueLightInformations>& lightInformations);
};
