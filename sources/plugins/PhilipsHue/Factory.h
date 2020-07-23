#pragma once
#include "IHueService.h"
#include "shared/event/EventHandler.hpp"
#include "UrlManager.h"
#include "IHueBridgeDiscovery.h"
#include "ILightManager.h"

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
   static boost::shared_ptr<ILightManager> createLightManager(boost::shared_ptr<CUrlManager>& urlManager);
};
