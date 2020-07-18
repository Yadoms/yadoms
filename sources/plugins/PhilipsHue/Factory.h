#pragma once
#include "IHueService.h"
#include "shared/event/EventHandler.hpp"

class CFactory
{
public:
   CFactory() = default;
   ~CFactory() = default;

   static boost::shared_ptr<IHueService> createHueService(shared::event::CEventHandler& mainEventHandler, int evtKeyStateReceived, int evtKeyStateTimeout);
};

