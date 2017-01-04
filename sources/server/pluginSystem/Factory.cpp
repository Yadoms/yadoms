#include "stdafx.h"
#include "Factory.h"
#include <shared/Log.h>
#include <shared/Executable.h>
#include "Information.h"
#include "Instance.h"
#include "InstanceStateHandler.h"
#include "yPluginApiImplementation.h"
#include "IQualifier.h"
#include "InvalidPluginException.hpp"
#include "IpcAdapter.h"
#include <shared/process/ProcessException.hpp>
#include "internalPlugin/Instance.h"
#include "internalPlugin/Information.h"
#include <shared/process/NativeExecutableCommandLine.h>
#include <shared/process/ProcessDeprecated.h>
#include <server/logging/ExternalProcessLogger.h>


namespace pluginSystem
{
   CFactory::CFactory(const IPathProvider& pathProvider,
                      boost::shared_ptr<shared::ILocation> locationProvider)
      : m_pathProvider(pathProvider),
        m_locationProvider(locationProvider)
   {
   }

   CFactory::~CFactory()
   {
   }

   boost::shared_ptr<const shared::plugin::information::IInformation> CFactory::createInformation(const std::string& pluginName) const
   {
      return boost::make_shared<CInformation>(m_pathProvider.pluginsPath() / pluginName);
   }

   boost::shared_ptr<IInstance> CFactory::createInstance(boost::shared_ptr<const database::entities::CPlugin> instanceData,
                                                         boost::shared_ptr<database::IDataProvider> dataProvider,
                                                         boost::shared_ptr<dataAccessLayer::IDataAccessLayer> dataAccessLayer,
                                                         boost::shared_ptr<IQualifier> qualifier,
                                                         boost::shared_ptr<IInstanceStoppedListener> instanceStoppedListener) const
   {
      if (instanceData->Id() == dataProvider->getPluginRequester()->getSystemInstance()->Id())
         return createInternalPluginInstance(instanceData,
                                             dataProvider,
                                             dataAccessLayer,
                                             qualifier,
                                             instanceStoppedListener);

      auto pluginInformation = createInformation(instanceData->Type());

      auto instanceStateHandler = createInstanceStateHandler(instanceData,
                                                             pluginInformation,
                                                             dataProvider,
                                                             dataAccessLayer,
                                                             qualifier,
                                                             instanceStoppedListener);

      auto logger = createProcessLogger(instanceData);

      auto yPluginApi = createInstanceRunningContext(pluginInformation,
                                                     instanceData,
                                                     instanceStateHandler,
                                                     dataProvider,
                                                     dataAccessLayer);

      auto commandLine = createCommandLine(pluginInformation,
                                           yPluginApi->id());

      auto process = createInstanceProcess(commandLine,
                                           logger,
                                           instanceStateHandler);

      return boost::make_shared<CInstance>(instanceData,
                                           pluginInformation,
                                           pluginDataPath(instanceData->Id()),
                                           process,
                                           yPluginApi);
   }

   boost::shared_ptr<IInstance> CFactory::createInternalPluginInstance(boost::shared_ptr<const database::entities::CPlugin> instanceData,
                                                                       boost::shared_ptr<database::IDataProvider> dataProvider,
                                                                       boost::shared_ptr<dataAccessLayer::IDataAccessLayer> dataAccessLayer,
                                                                       boost::shared_ptr<IQualifier> qualifier,
                                                                       boost::shared_ptr<IInstanceStoppedListener> instanceStoppedListener) const
   {
      auto pluginInformation = boost::make_shared<internalPlugin::CInformation>();

      auto instanceStateHandler = createInstanceStateHandler(instanceData,
                                                             pluginInformation,
                                                             dataProvider,
                                                             dataAccessLayer,
                                                             qualifier,
                                                             instanceStoppedListener);

      auto apiImplementation = createApiPluginImplementation(pluginInformation,
                                                             instanceData,
                                                             instanceStateHandler,
                                                             dataProvider,
                                                             dataAccessLayer);

      return boost::make_shared<internalPlugin::CInstance>(instanceData,
                                                           pluginInformation,
                                                           apiImplementation,
                                                           instanceStateHandler);
   }

   boost::filesystem::path CFactory::pluginLogFile(int instanceId) const
   {
      return m_pathProvider.pluginsLogPath() / std::to_string(instanceId) / "plugin.log";
   }

   boost::filesystem::path CFactory::pluginDataPath(int instanceId) const
   {
      auto pluginDataPath(m_pathProvider.pluginsDataPath() / std::to_string(instanceId));

      if (!boost::filesystem::exists(pluginDataPath))
         boost::filesystem::create_directory(pluginDataPath);

      return pluginDataPath;
   }

   boost::shared_ptr<shared::process::IExternalProcessLogger> CFactory::createProcessLogger(boost::shared_ptr<const database::entities::CPlugin> instanceData) const
   {
      return boost::make_shared<logging::CExternalProcessLogger>("plugin/" + instanceData->Type() + " #" + std::to_string(instanceData->Id()),
                                                                 pluginLogFile(instanceData->Id()));
   }

   boost::shared_ptr<CInstanceStateHandler> CFactory::createInstanceStateHandler(boost::shared_ptr<const database::entities::CPlugin> instanceData,
                                                                                 boost::shared_ptr<const shared::plugin::information::IInformation> pluginInformation,
                                                                                 boost::shared_ptr<database::IDataProvider> dataProvider,
                                                                                 boost::shared_ptr<dataAccessLayer::IDataAccessLayer> dataAccessLayer,
                                                                                 boost::shared_ptr<IQualifier> qualifier,
                                                                                 boost::shared_ptr<IInstanceStoppedListener> instanceStoppedListener) const
   {
      return boost::make_shared<CInstanceStateHandler>(instanceData,
                                                       pluginInformation,
                                                       dataProvider->getPluginRequester(),
                                                       qualifier,
                                                       dataProvider->getPluginEventLoggerRequester(),
                                                       dataAccessLayer->getAcquisitionHistorizer(),
                                                       instanceStoppedListener,
                                                       dataAccessLayer->getDeviceManager(),
                                                       dataAccessLayer->getKeywordManager());
   }

   boost::shared_ptr<shared::process::ICommandLine> CFactory::createCommandLine(const boost::shared_ptr<const shared::plugin::information::IInformation> pluginInformation,
                                                                                const std::string& messageQueueId) const
   {
      std::vector<std::string> args;
      args.push_back(messageQueueId);

      return boost::make_shared<shared::process::CNativeExecutableCommandLine>(pluginInformation->getPath() / shared::CExecutable::ToFileName(pluginInformation->getType()),
                                                                               ".",
                                                                               args);
   }

   boost::shared_ptr<shared::process::IProcess> CFactory::createInstanceProcess(boost::shared_ptr<shared::process::ICommandLine> commandLine,
                                                                                boost::shared_ptr<shared::process::IExternalProcessLogger> logger,
                                                                                boost::shared_ptr<CInstanceStateHandler> instanceStateHandler) const
   {
      try
      {
         return boost::make_shared<shared::process::CProcessDeprecated>(commandLine,
                                                                        instanceStateHandler,
                                                                        logger);
      }
      catch (shared::process::CProcessException& e)
      {
         YADOMS_LOG(error) << "Error starting plugin " << commandLine->executable() << " : " << e.what();
         instanceStateHandler->signalStartError(e.what());
         throw;
      }
   }

   boost::shared_ptr<CYPluginApiImplementation> CFactory::createApiPluginImplementation(boost::shared_ptr<const shared::plugin::information::IInformation> pluginInformation,
                                                                                        boost::shared_ptr<const database::entities::CPlugin> instanceData,
                                                                                        boost::shared_ptr<IInstanceStateHandler> instanceStateHandler,
                                                                                        boost::shared_ptr<database::IDataProvider> dataProvider,
                                                                                        boost::shared_ptr<dataAccessLayer::IDataAccessLayer> dataAccessLayer) const
   {
      return boost::make_shared<CYPluginApiImplementation>(pluginInformation,
                                                           instanceData,
                                                           pluginDataPath(instanceData->Id()),
                                                           instanceStateHandler,
                                                           dataProvider,
                                                           dataAccessLayer->getDeviceManager(),
                                                           dataAccessLayer->getKeywordManager(),
                                                           dataAccessLayer->getAcquisitionHistorizer(),
                                                           m_locationProvider);
   }

   boost::shared_ptr<IIpcAdapter> CFactory::createInstanceRunningContext(boost::shared_ptr<const shared::plugin::information::IInformation> pluginInformation,
                                                                         boost::shared_ptr<const database::entities::CPlugin> instanceData,
                                                                         boost::shared_ptr<IInstanceStateHandler> instanceStateHandler,
                                                                         boost::shared_ptr<database::IDataProvider> dataProvider,
                                                                         boost::shared_ptr<dataAccessLayer::IDataAccessLayer> dataAccessLayer) const
   {
      auto apiImplementation = createApiPluginImplementation(pluginInformation,
                                                             instanceData,
                                                             instanceStateHandler,
                                                             dataProvider,
                                                             dataAccessLayer);

      return boost::make_shared<CIpcAdapter>(apiImplementation);
   }

   bool CFactory::isValidPlugin(const boost::filesystem::path& directory) const
   {
      // Check if plugin is a native executable plugin
      const auto expectedExecutableName(shared::CExecutable::ToFileName(directory.filename().string()));

      for (boost::filesystem::directory_iterator fileIterator(directory); fileIterator != boost::filesystem::directory_iterator(); ++fileIterator)
      {
         if (boost::filesystem::is_regular_file(fileIterator->status()) && // It's a file...
            boost::iequals(fileIterator->path().filename().string(), expectedExecutableName)) // ...with the same name as sub-directory...
            return true;
      }

      return false;
   }

   std::vector<boost::filesystem::path> CFactory::findPluginDirectories() const
   {
      // Look for all subdirectories in plugin path directory, where it contains library with same name,
      // for example a subdirectory "fakePlugin" containing a "fakePlugin.dll|so" file
      std::vector<boost::filesystem::path> pluginDirectories;

      if (boost::filesystem::exists(m_pathProvider.pluginsPath()) && boost::filesystem::is_directory(m_pathProvider.pluginsPath()))
      {
         // Check all subdirectories in plugin path
         for (boost::filesystem::directory_iterator subDirIterator(m_pathProvider.pluginsPath());
              subDirIterator != boost::filesystem::directory_iterator();
              ++subDirIterator)
         {
            if (boost::filesystem::is_directory(subDirIterator->status()) && isValidPlugin(subDirIterator->path()))
               pluginDirectories.push_back(subDirIterator->path());
         }
      }

      return pluginDirectories;
   }

   IFactory::AvailablePluginMap CFactory::findAvailablePlugins() const
   {
      AvailablePluginMap availablePlugins;

      auto pluginDirectories = findPluginDirectories();

      for (auto pluginDirectory = pluginDirectories.begin();
           pluginDirectory != pluginDirectories.end(); ++pluginDirectory)
      {
         try
         {
            // Get informations for current found plugin
            auto pluginName = pluginDirectory->filename().string();

            auto pluginInformation = createInformation(pluginName);
            if (pluginInformation->isSupportedOnThisPlatform())
            {
               availablePlugins[pluginName] = pluginInformation;
               YADOMS_LOG(information) << "Plugin " << pluginName << " found";
            }
            else
            {
               YADOMS_LOG(warning) << "Plugin " << pluginName << " found but unsupported on this platform";
            }
         }
         catch (CInvalidPluginException& e)
         {
            // Invalid plugin
            YADOMS_LOG(warning) << e.what() << ", found in plugin path but is not a valid plugin";
         }
         catch (shared::exception::CInvalidParameter& e)
         {
            // Invalid plugin parameter
            YADOMS_LOG(warning) << "Invalid plugin parameter : " << e.what();
         }
         catch (shared::exception::CException& e)
         {
            // Fail to load one plugin
            YADOMS_LOG(warning) << "Invalid plugin : " << e.what();
         }
      }

      return availablePlugins;
   }
} // namespace pluginSystem


