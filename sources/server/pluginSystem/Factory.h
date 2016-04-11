#pragma once
#include "IFactory.h"
#include "InstanceStateHandler.h"
#include <shared/process/IProcess.h>
#include <shared/process/ILogger.h>
#include <shared/process/ICommandLine.h>
#include "IContextAccessor.h"

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
      boost::shared_ptr<IInstance> createInstance(boost::shared_ptr<const database::entities::CPlugin> instanceData,
                                                  boost::shared_ptr<database::IDataProvider> dataProvider,
                                                  boost::shared_ptr<dataAccessLayer::IDataAccessLayer> dataAccessLayer,
                                                  const boost::shared_ptr<IQualifier> qualifier,
                                                  boost::shared_ptr<shared::event::CEventHandler> managerEventHandler,
                                                  int normalStopEventId,
                                                  int abnormalStopEventId) const override;
      // [END] IFactory Implementation

   private:
      boost::shared_ptr<const shared::plugin::information::IInformation> createInformation(const std::string& pluginName) const;

      boost::shared_ptr<shared::process::ILogger> createProcessLogger(const std::string& pluginName) const;

      boost::shared_ptr<shared::process::ICommandLine> createCommandLine(const boost::shared_ptr<const shared::plugin::information::IInformation> pluginInformation,
                                                                         const std::string& messageQueueId) const;

      boost::shared_ptr<CInstanceStateHandler> createInstanceStateHandler(boost::shared_ptr<database::IPluginRequester> dbRequester,
                                                                          boost::shared_ptr<dataAccessLayer::IEventLogger> eventLogger,
                                                                          boost::shared_ptr<shared::event::CEventHandler> managerEventHandler,
                                                                          int instanceId,
                                                                          int normalStopEventId,
                                                                          int abnormalStopEventId) const;

      boost::shared_ptr<shared::process::IProcess> createInstanceProcess(boost::shared_ptr<shared::process::ICommandLine> commandLine,
                                                                         boost::shared_ptr<shared::process::ILogger> logger,
                                                                         boost::shared_ptr<CInstanceStateHandler> instanceStatteHandler) const;

      boost::shared_ptr<IContextAccessor> createInstanceRunningContext(boost::shared_ptr<const shared::plugin::information::IInformation> pluginInformation,
                                                                       boost::shared_ptr<const database::entities::CPlugin> instanceData,
                                                                       boost::shared_ptr<database::IDataProvider> dataProvider,
                                                                       boost::shared_ptr<dataAccessLayer::IDataAccessLayer> dataAccessLayer) const;

      boost::filesystem::path getPluginPath(const std::string& pluginName) const;

      bool isValidPlugin(const boost::filesystem::path& directory) const;
      std::vector<boost::filesystem::path> findPluginDirectories() const;


      const boost::filesystem::path m_pluginPath;
   };
} // namespace pluginSystem
