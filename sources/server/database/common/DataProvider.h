#pragma once
#include "database/IDataProvider.h"
#include "database/IDatabaseRequester.h"
#include "requesters/Plugin.h"
#include "requesters/Configuration.h"
#include "requesters/Device.h"
#include "requesters/Keyword.h"
#include "requesters/Page.h"
#include "requesters/Rule.h"
#include "requesters/Widget.h"
#include "requesters/PluginEventLogger.h"
#include "requesters/EventLogger.h"
#include "requesters/Acquisition.h"
#include "requesters/Recipient.h"
#include <Poco/Util/Timer.h>
#include <shared/versioning/Version.h>


namespace database
{
   namespace common
   {
      //--------------------------------------------------------------
      /// \Brief		   Database provider
      //--------------------------------------------------------------
      class CDataProvider : public IDataProvider
      {
      public:
         //--------------------------------------------------------------
         /// \Brief		   Constructor
         /// \param [in]	databaseRequester: The database engine
         //--------------------------------------------------------------
         explicit CDataProvider(boost::shared_ptr<IDatabaseRequester> databaseRequester);

         //--------------------------------------------------------------
         /// \Brief		   Destructor
         //--------------------------------------------------------------
         virtual ~CDataProvider();

         // IDatabaseProvider implementation
         bool load() override;
         void unload() override;
         void stopMaintenanceTasks() override;
         boost::shared_ptr<ITransactionalProvider> getTransactionalEngine() override;

         boost::shared_ptr<IPluginRequester> getPluginRequester() override
         {
            return m_pluginRequester;
         }

         boost::shared_ptr<IConfigurationRequester> getConfigurationRequester() override
         {
            return m_configurationRequester;
         }

         boost::shared_ptr<IDeviceRequester> getDeviceRequester() override
         {
            return m_deviceRequester;
         }

         boost::shared_ptr<IKeywordRequester> getKeywordRequester() override
         {
            return m_keywordRequester;
         }

         boost::shared_ptr<IPageRequester> getPageRequester() override
         {
            return m_pageRequester;
         }

         boost::shared_ptr<IWidgetRequester> getWidgetRequester() override
         {
            return m_widgetRequester;
         }

         boost::shared_ptr<IPluginEventLoggerRequester> getPluginEventLoggerRequester() override
         {
            return m_pluginEventLoggerRequester;
         }

         boost::shared_ptr<IEventLoggerRequester> getEventLoggerRequester() override
         {
            return m_eventLoggerRequester;
         }

         boost::shared_ptr<IAcquisitionRequester> getAcquisitionRequester() override
         {
            return m_acquisitionRequester;
         }

         boost::shared_ptr<IRuleRequester> getRuleRequester() override
         {
            return m_ruleRequester;
         }

         boost::shared_ptr<IRecipientRequester> getRecipientRequester() override
         {
            return m_recipientRequester;
         }

         boost::shared_ptr<IDatabaseRequester> getDatabaseRequester() override
         {
            return m_databaseRequester;
         }

         // [END] IDatabaseProvider implementation

         shared::versioning::CVersion getVersion() const;
      private:
         //--------------------------------------------------------------
         /// \Brief		Load all requesters
         //--------------------------------------------------------------
         void loadRequesters();

         //--------------------------------------------------------------
         /// \Brief		Initialize the maintenance tasks
         //--------------------------------------------------------------
         void initializeMaintenanceTasks();

         //--------------------------------------------------------------
         /// \Brief		Initialize the summary computing task
         //--------------------------------------------------------------
         void initializeSummaryComputingTask();

         //--------------------------------------------------------------
         /// \Brief		Initialize the purge task
         //--------------------------------------------------------------
         void initializePurgeTask();


         //--------------------------------------------------------------
         /// \Brief		Plugin requester
         //--------------------------------------------------------------
         boost::shared_ptr<requesters::CPlugin> m_pluginRequester;

         //--------------------------------------------------------------
         /// \Brief		Plugin requester
         //--------------------------------------------------------------
         boost::shared_ptr<requesters::CConfiguration> m_configurationRequester;

         //--------------------------------------------------------------
         /// \Brief		Device requester
         //--------------------------------------------------------------
         boost::shared_ptr<requesters::CDevice> m_deviceRequester;

         //--------------------------------------------------------------
         /// \Brief		Keyword requester
         //--------------------------------------------------------------
         boost::shared_ptr<requesters::CKeyword> m_keywordRequester;

         //--------------------------------------------------------------
         /// \Brief		Page requester
         //--------------------------------------------------------------
         boost::shared_ptr<requesters::CPage> m_pageRequester;

         //--------------------------------------------------------------
         /// \Brief		Widget requester
         //--------------------------------------------------------------
         boost::shared_ptr<requesters::CWidget> m_widgetRequester;

         //--------------------------------------------------------------
         /// \Brief		PluginEventLogger requester
         //--------------------------------------------------------------
         boost::shared_ptr<requesters::CPluginEventLogger> m_pluginEventLoggerRequester;

         //--------------------------------------------------------------
         /// \Brief		EventLogger requester
         //--------------------------------------------------------------
         boost::shared_ptr<requesters::CEventLogger> m_eventLoggerRequester;

         //--------------------------------------------------------------
         /// \Brief		Acquisition requester
         //--------------------------------------------------------------
         boost::shared_ptr<requesters::CAcquisition> m_acquisitionRequester;

         //--------------------------------------------------------------
         /// \Brief		Rule requester
         //--------------------------------------------------------------
         boost::shared_ptr<requesters::CRule> m_ruleRequester;

         //--------------------------------------------------------------
         /// \Brief		Recipient requester
         //--------------------------------------------------------------
         boost::shared_ptr<requesters::CRecipient> m_recipientRequester;

         //--------------------------------------------------------------
         /// \Brief		database requester
         //--------------------------------------------------------------
         boost::shared_ptr<IDatabaseRequester> m_databaseRequester;

         //--------------------------------------------------------------
         /// \Brief		The maintenance timer
         //--------------------------------------------------------------
         boost::shared_ptr<Poco::Util::Timer> m_maintenanceTimer;

         //--------------------------------------------------------------
         /// \Brief		The maintenance summary computing task
         //--------------------------------------------------------------
         Poco::Util::TimerTask::Ptr m_maintenanceSummaryComputingTask;

         //--------------------------------------------------------------
         /// \Brief		The maintenance purge task
         //--------------------------------------------------------------
         Poco::Util::TimerTask::Ptr m_maintenancePurgeTask;
      };
   } //namespace common
} //namespace database
