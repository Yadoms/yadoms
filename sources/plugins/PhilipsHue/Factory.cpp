#include "Factory.h"
#include "HueService.h"

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
