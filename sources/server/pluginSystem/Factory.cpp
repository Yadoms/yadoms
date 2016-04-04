#include "stdafx.h"
#include "Factory.h"
#include "Information.h"
#include "Instance.h"
#include "InstanceStateHandler.h"
#include "yPluginApiImplementation.h"
#include "IQualifier.h"
#include "PluginException.hpp"
#include <shared/Log.h>
#include "NativeExecutableCommandLine.h"
#include "Process.h"
#include "Runner.h"

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
   return boost::make_shared<CInformation>(pluginPath);
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
   boost::shared_ptr<IInstanceStateHandler> instanceStateHandler = createInstanceStateHandler(dataProvider->getPluginRequester(),
                                                                                              instanceData->Id());

   boost::shared_ptr<shared::process::ILogger> logger = createProcessLogger();

   boost::shared_ptr<CYPluginApiImplementation> yPluginApi(boost::make_shared<CYPluginApiImplementation>( //TODO déplacer la construction dans m_factory
      pluginInformation,
      instanceData,
      dataProvider,
      deviceManager,
      acquisitionHistorizer));

   boost::shared_ptr<ICommandLine> commandLine = createCommandLine(pluginInformation,
                                                                   std::string()/*TODO*/);

   boost::shared_ptr<shared::process::IRunner> runner = createInstanceRunner(commandLine,
                                                                             logger,
                                                                             instanceStateHandler);

   return boost::make_shared<CInstance>(pluginInformation,
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

boost::shared_ptr<ICommandLine> CFactory::createCommandLine(const boost::shared_ptr<const shared::plugin::information::IInformation> pluginInformation,
                                                            const std::string& messageQueueId) const
{
   std::vector<std::string> args;
   args.push_back(messageQueueId);

   return boost::make_shared<CNativeExecutableCommandLine>(pluginInformation->getPath(),
                                                           pluginInformation->getType(),
                                                           args);
}

boost::shared_ptr<shared::process::IProcess> CFactory::createProcess(boost::shared_ptr<ICommandLine> commandLine,
                                                                     boost::shared_ptr<shared::process::ILogger> logger,
                                                                     boost::shared_ptr<IInstanceStateHandler> stopNotifier) const
{
   try
   {
      return boost::make_shared<CProcess>(commandLine, logger);

   }
   catch (CPluginException& e)
   {
      logger->log((boost::format("Error starting plugin %1% : %2%") % commandLine->executable() % e.what()).str());
      stopNotifier->signalStartError(e.what());
      throw;
   }
}

boost::shared_ptr<shared::process::IRunner> CFactory::createInstanceRunner(boost::shared_ptr<ICommandLine> commandLine,
                                                                           boost::shared_ptr<shared::process::ILogger> logger,
                                                                           boost::shared_ptr<IInstanceStateHandler> stopNotifier) const
{
   logger->log("#### START ####");

   boost::shared_ptr<shared::process::IProcess> process = createProcess(commandLine, logger, stopNotifier);
   return boost::make_shared<CRunner>(process, logger, stopNotifier);
}

} // namespace pluginSystem
