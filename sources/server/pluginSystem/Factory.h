#pragma once
#include "IFactory.h"
#include "ICommandLine.h"
#include <shared/shared/process/IRunner.h>
#include <shared/shared/process/IProcess.h>

namespace pluginSystem
{
   //--------------------------------------------------------------
   /// \brief	IFactory implementation
   //--------------------------------------------------------------
   class CFactory : public IFactory
   {
   public:
      //--------------------------------------------------------------
      /// \brief	Constructor
      /// \param[in] pluginPath     The main plugins path
      //--------------------------------------------------------------
      explicit CFactory(const boost::filesystem::path& pluginPath);

      //--------------------------------------------------------------
      /// \brief	Destructor
      //--------------------------------------------------------------
      virtual ~CFactory();

      // IFactory Implementation //TODO il y a peut-être des fonctions à supprimer de l'interface et à rendre privées
      AvailablePluginMap findAvailablePlugins() const override;

      boost::shared_ptr<const shared::plugin::information::IInformation> createInformation(const std::string& pluginName) const override;

      boost::shared_ptr<shared::process::ILogger> createProcessLogger() const override;

      boost::shared_ptr<IInstance> createInstance(boost::shared_ptr<const database::entities::CPlugin> instanceData,
                                                  boost::shared_ptr<database::IDataProvider> dataProvider,
                                                  boost::shared_ptr<dataAccessLayer::IDataAccessLayer> dataAccessLayer,
                                                  const boost::shared_ptr<IQualifier> qualifier,
                                                  boost::shared_ptr<shared::event::CEventHandler> managerEventHandler) const override;

      boost::shared_ptr<IInstanceStateHandler> createInstanceStateHandler(boost::shared_ptr<database::IPluginRequester> dbRequester,
                                                                          boost::shared_ptr<dataAccessLayer::IEventLogger> eventLogger,
                                                                          boost::shared_ptr<shared::event::CEventHandler> managerEventHandler,
                                                                          int instanceId) const override;
      // [END] IFactory Implementation

   private:
      boost::shared_ptr<ICommandLine> createCommandLine(const boost::shared_ptr<const shared::plugin::information::IInformation> pluginInformation,
                                                        const std::string& messageQueueId) const;

      boost::shared_ptr<shared::process::IProcess> createProcess(boost::shared_ptr<ICommandLine> commandLine,
                                                                 boost::shared_ptr<shared::process::ILogger> logger,
                                                                 boost::shared_ptr<IInstanceStateHandler> stopNotifier) const;

      boost::shared_ptr<shared::process::IRunner> createInstanceRunner(boost::shared_ptr<ICommandLine> commandLine,
                                                                       boost::shared_ptr<shared::process::ILogger> logger,
                                                                       boost::shared_ptr<IInstanceStateHandler> stopNotifier) const;

      boost::filesystem::path getPluginPath(const std::string& pluginName) const;

      std::vector<boost::filesystem::path> findPluginDirectories() const;


      const boost::filesystem::path& m_pluginPath;
   };
} // namespace pluginSystem
