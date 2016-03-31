#include "stdafx.h"
#include "Factory.h"
#include "Information.h"
#include "Instance.h"
#include "InstanceStateHandler.h"
#include "yPluginApiImplementation.h"
#include "IQualifier.h"
#include "PluginException.hpp"
#include <shared/Log.h>
#include "NativeExecutableLoader.h"

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
   boost::shared_ptr<const database::entities::CPlugin> instanceData,
   boost::shared_ptr<database::IDataProvider> dataProvider,
   boost::shared_ptr<dataAccessLayer::IDeviceManager> deviceManager,
   boost::shared_ptr<dataAccessLayer::IAcquisitionHistorizer> acquisitionHistorizer,
   const boost::shared_ptr<IQualifier> qualifier,
   boost::shared_ptr<shared::event::CEventHandler> supervisor,
   int pluginManagerEventId) const
{
   boost::shared_ptr<IInstanceStateHandler> stopNotifier = createInstanceStateHandler(m_ruleStateHandler, m_ruleData->Id());

   boost::shared_ptr<shared::process::ILogger> logger = boost::shared_ptr<shared::process::ILogger>(); // TODO remettre "m_factory->createProcessLogger()"; // TODO m_factory est l'équivalent du automation::script::IManager

   boost::shared_ptr<CYPluginApiImplementation> yPluginApi(boost::make_shared<CYPluginApiImplementation>( //TODO déplacer la construction dans m_factory
      pluginInformation,
      instanceData,
      dataProvider,
      deviceManager,
      acquisitionHistorizer));

   boost::shared_ptr<shared::process::IRunner> runner = createInstanceRunner(pluginInformation,
                                                                             logger,
                                                                             std::string(), //TODO voir ID des règles
                                                                             stopNotifier);

   return boost::make_shared<CInstance>(pluginInformation,
                                        pluginData,
                                        dataProvider,
                                        deviceManager,
                                        acquisitionHistorizer,
                                        qualifier,
                                        supervisor,
                                        pluginManagerEventId,
                                        runner);
}

boost::shared_ptr<shared::process::ILogger> CFactory::createProcessLogger() const
{
   return boost::shared_ptr<shared::process::ILogger>();

   //TODO
   //boost::shared_ptr<shared::process::ILogger> logger(new CLogger(scriptPath));
   //return logger;
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

boost::shared_ptr<shared::process::IRunner> CFactory::createInstanceRunner(const boost::shared_ptr<const shared::plugin::information::IInformation> pluginInformation,
                                                                           boost::shared_ptr<shared::process::ILogger> logger,
                                                                           const std::string& apiContextId,
                                                                           boost::shared_ptr<IInstanceStateHandler> stopNotifier) const
{
   try
   {
      return getAssociatedLoader(pluginInformation)->createRunner(pluginInformation,
                                                                  logger,
                                                                  apiContextId,
                                                                  stopNotifier);
   }
   catch (shared::exception::COutOfRange& e)
   {
      YADOMS_LOG(error) << "Plugin type is not supported : " << e.what();
      throw CPluginException("plugin type is not supported");
   }
}

boost::shared_ptr<ILoader> CFactory::getAssociatedLoader(
   const boost::shared_ptr<const shared::plugin::information::IInformation> pluginInformation) const
{
   //TODO actualy only one loader supported
   return boost::make_shared<CNativeExecutableLoader>();
}

} // namespace pluginSystem
