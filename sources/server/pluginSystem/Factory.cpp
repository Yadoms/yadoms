#include "stdafx.h"
#include "Factory.h"
#include <shared/Log.h>
#include <shared/Executable.h>
#include "Information.h"
#include "Instance.h"
#include "InstanceStateHandler.h"
#include "yPluginApiImplementation.h"
#include "IQualifier.h"
#include "NativeExecutableCommandLine.h"
#include "InvalidPluginException.hpp"
#include "ContextAccessor.h"
#include <shared/process/Process.h>
#include <shared/process/ProcessException.hpp>
#include <shared/FileSystemExtension.h>
#include <shared/process/Logger.h>


namespace pluginSystem
{
   CFactory::CFactory(const boost::filesystem::path& pluginPath)
      : m_pluginPath(pluginPath)
   {
   }

   CFactory::~CFactory()
   {
   }

   boost::shared_ptr<const shared::plugin::information::IInformation> CFactory::createInformation(const std::string& pluginName) const
   {
      return boost::make_shared<CInformation>(getPluginPath(pluginName));
   }

   boost::shared_ptr<IInstance> CFactory::createInstance(boost::shared_ptr<const database::entities::CPlugin> instanceData,
                                                         boost::shared_ptr<database::IDataProvider> dataProvider,
                                                         boost::shared_ptr<dataAccessLayer::IDataAccessLayer> dataAccessLayer,
                                                         const boost::shared_ptr<IQualifier> qualifier, //TODO faut s'en servir !
                                                         boost::shared_ptr<IInstanceStoppedListener> instanceStoppedListener) const
   {
      auto pluginInformation = createInformation(instanceData->Type());

      auto instanceStateHandler = createInstanceStateHandler(instanceData,
                                                             pluginInformation,
                                                             dataProvider,
                                                             dataAccessLayer,
                                                             instanceStoppedListener);

      auto logger = createProcessLogger(instanceData->Type());

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
                                           process,
                                           yPluginApi);
   }

   boost::filesystem::path CFactory::pluginLogFile(const std::string& pluginName) const
   {
      return m_pluginPath / pluginName / "yadomsPlugin.log";
   }

   boost::shared_ptr<shared::process::ILogger> CFactory::createProcessLogger(const std::string& pluginName) const
   {
      return boost::make_shared<shared::process::CLogger>("plugin/" + pluginName,
                                                          pluginLogFile(pluginName));
   }

   boost::shared_ptr<CInstanceStateHandler> CFactory::createInstanceStateHandler(boost::shared_ptr<const database::entities::CPlugin> instanceData,
                                                                                 boost::shared_ptr<const shared::plugin::information::IInformation> pluginInformation,
                                                                                 boost::shared_ptr<database::IDataProvider> dataProvider,
                                                                                 boost::shared_ptr<dataAccessLayer::IDataAccessLayer> dataAccessLayer,
                                                                                 boost::shared_ptr<IInstanceStoppedListener> instanceStoppedListener) const
   {
      return boost::make_shared<CInstanceStateHandler>(instanceData,
                                                       pluginInformation,
                                                       dataProvider->getPluginRequester(),
                                                       dataAccessLayer->getEventLogger(),
                                                       dataProvider->getPluginEventLoggerRequester(),
                                                       dataAccessLayer->getAcquisitionHistorizer(),
                                                       instanceStoppedListener,
                                                       dataAccessLayer->getDeviceManager(),
                                                       dataProvider->getKeywordRequester());
   }

   boost::shared_ptr<shared::process::ICommandLine> CFactory::createCommandLine(const boost::shared_ptr<const shared::plugin::information::IInformation> pluginInformation,
                                                                                const std::string& messageQueueId) const
   {
      std::vector<std::string> args;
      args.push_back(messageQueueId);

      return boost::make_shared<CNativeExecutableCommandLine>(pluginInformation->getPath(),
                                                              pluginInformation->getType(),
                                                              args);
   }

   boost::shared_ptr<shared::process::IProcess> CFactory::createInstanceProcess(boost::shared_ptr<shared::process::ICommandLine> commandLine,
                                                                                boost::shared_ptr<shared::process::ILogger> logger,
                                                                                boost::shared_ptr<CInstanceStateHandler> instanceStateHandler) const
   {
      try
      {
         return boost::make_shared<shared::process::CProcess>(commandLine,
                                                              shared::CFileSystemExtension::getModulePath().string(),
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

   boost::shared_ptr<IContextAccessor> CFactory::createInstanceRunningContext(boost::shared_ptr<const shared::plugin::information::IInformation> pluginInformation,
                                                                              boost::shared_ptr<const database::entities::CPlugin> instanceData,
                                                                              boost::shared_ptr<IInstanceStateHandler> instanceStateHandler,
                                                                              boost::shared_ptr<database::IDataProvider> dataProvider,
                                                                              boost::shared_ptr<dataAccessLayer::IDataAccessLayer> dataAccessLayer) const
   {
      auto apiImplementation(boost::make_shared<CYPluginApiImplementation>(pluginInformation,
                                                                           instanceData,
                                                                           instanceStateHandler,
                                                                           dataProvider,
                                                                           dataAccessLayer->getDeviceManager(),
                                                                           dataAccessLayer->getAcquisitionHistorizer()));

      return boost::make_shared<CContextAccessor>(apiImplementation);
   }


   boost::filesystem::path CFactory::getPluginPath(const std::string& pluginName) const
   {
      auto path(m_pluginPath);
      path /= pluginName;
      return path;
   }

   bool CFactory::isValidPlugin(const boost::filesystem::path& directory) const
   {
      // Check if plugin is a native executable plugin //TODO ça serait mieux dans une classe à part
      const auto expectedLibName(shared::CExecutable::ToFileName(directory.filename().string()));

      for (boost::filesystem::directory_iterator fileIterator(directory); fileIterator != boost::filesystem::directory_iterator(); ++fileIterator)
      {
         if (boost::filesystem::is_regular_file(fileIterator->status()) && // It's a file...
            boost::iequals(fileIterator->path().filename().string(), expectedLibName)) // ...with the same name as sub-directory...
            return true;
      }

      return false;
   }

   std::vector<boost::filesystem::path> CFactory::findPluginDirectories() const
   {
      // Look for all subdirectories in m_pluginPath directory, where it contains library with same name,
      // for example a subdirectory "fakePlugin" containing a "fakePlugin.dll|so" file
      std::vector<boost::filesystem::path> pluginDirectories;

      if (boost::filesystem::exists(m_pluginPath) && boost::filesystem::is_directory(m_pluginPath))
      {
         // Check all subdirectories in m_pluginPath
         for (boost::filesystem::directory_iterator subDirIterator(m_pluginPath); subDirIterator != boost::filesystem::directory_iterator(); ++subDirIterator)
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
         catch (shared::exception::CException & e)
         {
            // Fail to load one plugin
            YADOMS_LOG(warning) << "Invalid plugin : " << e.what();
         }
      }

      return availablePlugins;
   }
} // namespace pluginSystem
