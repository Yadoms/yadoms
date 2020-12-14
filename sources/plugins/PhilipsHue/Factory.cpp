#include "Factory.h"
#include "HueService.h"
#include "HueBridgeDiscovery.h"
#include "LightManager.h"

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

boost::shared_ptr<ILightManager> CFactory::createLightManager(boost::shared_ptr<CUrlManager>& urlManager)
{
   return boost::make_shared<CLightManager>(urlManager);
}
