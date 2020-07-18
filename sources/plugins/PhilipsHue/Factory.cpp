#include "Factory.h"
#include "HueService.h"

boost::shared_ptr<IHueService> CFactory::createHueService(shared::event::CEventHandler& mainEventHandler,
                                                          CHueBridgeDiscovery::HueInformations& hubInformations,
                                                          int evtKeyStateReceived,
                                                          int evtKeyStateTimeout)
{
   return boost::make_shared<CHueService>(mainEventHandler, hubInformations, evtKeyStateReceived, evtKeyStateTimeout);
}
