#pragma once
#include "IHueService.h"
#include "shared/event/EventHandler.hpp"
#include "HueBridgeDiscovery.h"
#include "UrlManager.h"

class CFactory
{
public:
   CFactory() = default;
   ~CFactory() = default;

   static boost::shared_ptr<IHueService> createHueService(shared::event::CEventHandler& mainEventHandler,
                                                          CHueBridgeDiscovery::HueInformations& hubInformations,
                                                          int evtKeyStateReceived,
                                                          int evtKeyStateTimeout,
                                                          boost::shared_ptr<CUrlManager>& urlManager);
};
