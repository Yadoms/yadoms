#include "stdafx.h"
#include "Factory.h"
#include "Information.h"
#include "Instance.h"
#include "InstanceStateHandler.h"

namespace pluginSystem
{

CFactory::CFactory()
{
}

CFactory::~CFactory()
{
}

boost::shared_ptr<const shared::plugin::information::IInformation> CFactory::createInformation(const boost::filesystem::path& pluginPath) const
{
   return boost::make_shared<pluginSystem::CInformation>(pluginPath);
}

boost::shared_ptr<IInstance> CFactory::createInstance(
   const boost::shared_ptr<const shared::plugin::information::IInformation> pluginInformation,
   boost::shared_ptr<const database::entities::CPlugin> pluginData,
   boost::shared_ptr<database::IDataProvider> dataProvider,
   boost::shared_ptr<dataAccessLayer::IDeviceManager> deviceManager,
   boost::shared_ptr<dataAccessLayer::IAcquisitionHistorizer> acquisitionHistorizer,
   const boost::shared_ptr<IQualifier> qualifier,
   boost::shared_ptr<shared::event::CEventHandler> supervisor,
   int pluginManagerEventId) const
{
   return boost::make_shared<CInstance>(shared_from_this(),
                                        pluginInformation,
                                        pluginData,
                                        dataProvider,
                                        deviceManager,
                                        acquisitionHistorizer,
                                        qualifier,
                                        supervisor,
                                        pluginManagerEventId);
}

boost::shared_ptr<IInstanceStateHandler> CFactory::createInstanceStateHandler(
   boost::shared_ptr<database::IPluginRequester> dbRequester,
   boost::shared_ptr<dataAccessLayer::IEventLogger> eventLogger,
   boost::shared_ptr<shared::event::CEventHandler> managerEventHandler,
   int instanceId) const
{
   return boost::make_shared<CInstanceStateHandler>(dbRequester,
                                                    eventLogger,
                                                    managerEventHandler,
                                                    instanceId);
}

} // namespace pluginSystem
