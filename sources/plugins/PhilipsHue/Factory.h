#pragma once
#include "IHueService.h"
#include "shared/event/EventHandler.hpp"
#include "HueBridgeDiscovery.h"

class CFactory
{
public:
   CFactory() = default;
   ~CFactory() = default;

   static boost::shared_ptr<IHueService> createHueService(shared::event::CEventHandler& mainEventHandler,
                                                          CHueBridgeDiscovery::HueInformations& hubInformations,
                                                          int evtKeyStateReceived,
                                                          int evtKeyStateTimeout);
};
