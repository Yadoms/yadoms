#include "stdafx.h"
#include "DataProvider.h"
#include "versioning/VersionUpgraderFactory.h"
#include "versioning/VersionException.h"
#include "DatabaseTables.h"
#include "adapters/SingleValueAdapter.hpp"
#include "SummaryDataTask.h"
#include "PurgeTask.h"
#include <Poco/DateTime.h>
#include <shared/Log.h>
#include "database/DatabaseException.hpp"
#include "requesters/SerialPort.h"

namespace database
{
   namespace common
   {
      CDataProvider::CDataProvider(boost::shared_ptr<IDatabaseRequester> databaseRequester)
         : m_databaseRequester(databaseRequester),
           m_maintenanceTimer(boost::make_shared<Poco::Util::Timer>())
      {
      }

      CDataProvider::~CDataProvider()
      {
         CDataProvider::stopMaintenanceTasks();
         CDataProvider::unload();
      }

      shared::versioning::CSemVer CDataProvider::getVersion() const
      {
         try
         {
            if (!m_databaseRequester->checkTableExists(CConfigurationTable::getTableName())
               && !m_databaseRequester->checkTableExists(CDatabaseTable("Configuration")))
            {
               // New database
               return shared::versioning::CSemVer();
            }

            // Configuration table changed from Database 4.2.0 version (updated to 4.2.0 in Yadoms 2.1.0 version).
            // As database version is stored in the Configuration table and this table structure changed,
            // we have to know the current table structure before get Database version
            // To check table structure, just check if "Database" record exists (if yes, database is from old version)
            auto newDatabaseQuery = m_databaseRequester->newQuery();
            newDatabaseQuery->SelectCount().
                              From(CConfigurationTable::getTableName()).
                              Where(CConfigurationTable::getSectionColumnName(), CQUERY_OP_EQUAL, "Database");
            const auto isOldDatabase = m_databaseRequester->queryCount(*newDatabaseQuery) == 1;

            if (isOldDatabase)
            {
               auto qVersion = m_databaseRequester->newQuery();
               qVersion->Select(CDatabaseColumn("value")).
                         From(CDatabaseTable("Configuration")).
                         Where(CDatabaseColumn("section"), CQUERY_OP_EQUAL, "Database").
                         And(CDatabaseColumn("name"), CQUERY_OP_EQUAL, "Version");

               adapters::CSingleValueAdapter<std::string> adapter;
               m_databaseRequester->queryEntities(&adapter, *qVersion);
               auto results = adapter.getResults();

               if (!results.empty())
                  return shared::versioning::CSemVer(results[0]);
            }
            else
            {
               auto qVersion = m_databaseRequester->newQuery();
               qVersion->Select(CConfigurationTable::getValueColumnName()).
                         From(CConfigurationTable::getTableName()).
                         Where(CConfigurationTable::getSectionColumnName(), CQUERY_OP_EQUAL, "databaseVersion");

               adapters::CSingleValueAdapter<std::string> adapter;
               m_databaseRequester->queryEntities(&adapter, *qVersion);
               auto results = adapter.getResults();

               if (!results.empty())
                  return shared::versioning::CSemVer(results[0]);
            }
         }
         catch (std::exception& ex)
         {
            YADOMS_LOG(debug) << "Fail to get version of database : " << ex.what();
         }
         catch (...)
         {
            YADOMS_LOG(debug) << "Fail to get version of database : Unknown exception";
         }

         return shared::versioning::CSemVer();
      }

      // IDatabaseProvider implementation
      bool CDataProvider::load()
      {
         YADOMS_LOG(information) << "Load database";

         bool result;
         try
         {
            //initialize database engine
            m_databaseRequester->initialize();

            //check for update
            YADOMS_LOG(information) << "Check for database update...";

            versioning::CVersionUpgraderFactory::GetUpgrader()->checkForUpgrade(m_databaseRequester, getVersion());

            //create entities requester (high level queries)
            loadRequesters();

            YADOMS_LOG(information) << "Load database with success";
            result = true;

            //database loaded, start maintenance task
            initializeMaintenanceTasks();
         }
         catch (versioning::CVersionException& exc)
         {
            YADOMS_LOG(error) << "Fail to load database (upgrade error) : " << std::endl << exc.what();
            result = false;
         }
         catch (CDatabaseException& dbException)
         {
            YADOMS_LOG(error) << "Database exception while loading database" << std::endl << dbException.what();
            result = false;
         }
         catch (std::exception& ex)
         {
            YADOMS_LOG(error) << "Exception while loading database" << std::endl << ex.what();
            result = false;
         }
         catch (...)
         {
            YADOMS_LOG(error) << "Unknown exception while loading database";
            result = false;
         }
         return result;
      }

      void CDataProvider::initializeMaintenanceTasks()
      {
         initializeSummaryComputingTask();
         initializePurgeTask();
      }

      void CDataProvider::initializeSummaryComputingTask()
      {
         //create the summary computing task
         m_maintenanceSummaryComputingTask = Poco::Util::TimerTask::Ptr(new CSummaryDataTask(getAcquisitionRequester(), getKeywordRequester()));

         //schedule task now (at app start)
         m_maintenanceTimer->schedule(m_maintenanceSummaryComputingTask, Poco::Timestamp());

         //then schedule it to be run each hour (+1 minute in order to give a small latency for plugins): 00h01, 01h01, 02h01, 03h01, 04h01....
         //working with Poco::DateTime because using Poco::Util::Timer class
         Poco::DateTime now;
         Poco::DateTime firstOccurence(now.year(), now.month(), now.day(), now.hour(), 1);
         Poco::Timespan oneHourOffset(0, 1, 0, 0, 0);
         firstOccurence += oneHourOffset; //+1hour
         Poco::Timespan timeToWaitBeforeFirstOccurrence = firstOccurence - now;

         YADOMS_LOG(information) << "Summary task will happens @" << firstOccurence.hour() << ":" << firstOccurence.minute();
         YADOMS_LOG(information) << "Next step in " << timeToWaitBeforeFirstOccurrence.minutes() << " minutes";

         //schedule the timer to launch task each hour +1min
         //Schedule : now and each hour (1000 * 3600)
         const auto msWait = static_cast<long>(timeToWaitBeforeFirstOccurrence.totalMilliseconds());
         //force cast because value is maximum 1hour = 1000*3600 which is less than "long" maximum value
         const auto msWaitPeriod = static_cast<long>(oneHourOffset.totalMilliseconds());
         //force cast because value is 1 hour = 1000*3600 which is less than "long" maximum value

         m_maintenanceTimer->scheduleAtFixedRate(m_maintenanceSummaryComputingTask, msWait, msWaitPeriod);
      }

      void CDataProvider::initializePurgeTask()
      {
         //create the purge task
         m_maintenancePurgeTask = Poco::Util::TimerTask::Ptr(new CPurgeTask(getAcquisitionRequester(), m_databaseRequester));

         //schedule task now (at app start)
         m_maintenanceTimer->schedule(m_maintenancePurgeTask, Poco::Timestamp());

         //schedule task now (at app start)
         //working with Poco::DateTime because using Poco::Util::Timer class
         Poco::DateTime now;
         Poco::DateTime firstPurgeOccurence(now.year(), now.month(), now.day(), now.hour(), 15);
         Poco::Timespan oneDayOffset(24, 0, 0, 0, 0);
         firstPurgeOccurence += oneDayOffset; //+1day
         Poco::Timespan timeToWaitBeforeFirstPurgeOccurrence = firstPurgeOccurence - now;

         const auto msWait = static_cast<long>(timeToWaitBeforeFirstPurgeOccurrence.totalMilliseconds());
         //force cast because value is maximum 1hour = 1000*3600 which is less than "long" maximum value
         const auto msWaitPeriod = static_cast<long>(oneDayOffset.totalMilliseconds());
         //force cast because value is 1 hour = 1000*3600 which is less than "long" maximum value

         m_maintenanceTimer->scheduleAtFixedRate(m_maintenancePurgeTask, msWait, msWaitPeriod);
      }


      void CDataProvider::unload()
      {
         //initialize database engine
         if (m_databaseRequester)
            m_databaseRequester->finalize();
      }

      void CDataProvider::stopMaintenanceTasks()
      {
         //cancel all tasks
         if (m_maintenanceSummaryComputingTask)
            m_maintenanceSummaryComputingTask->cancel();
         if (m_maintenancePurgeTask)
            m_maintenancePurgeTask->cancel();
         m_maintenanceTimer.reset();

         m_maintenancePurgeTask.assign(nullptr);
         m_maintenanceSummaryComputingTask.assign(nullptr);
      }

      void CDataProvider::loadRequesters()
      {
         m_pluginRequester = boost::make_shared<requesters::CPlugin>(m_databaseRequester);
         m_configurationRequester = boost::make_shared<requesters::CConfiguration>(m_databaseRequester);
         m_deviceRequester = boost::make_shared<requesters::CDevice>(m_databaseRequester);
         m_keywordRequester = boost::make_shared<requesters::CKeyword>(m_databaseRequester);
         m_pageRequester = boost::make_shared<requesters::CPage>(m_databaseRequester);
         m_widgetRequester = boost::make_shared<requesters::CWidget>(m_databaseRequester);
         m_pluginEventLoggerRequester = boost::make_shared<requesters::CPluginEventLogger>(m_databaseRequester);
         m_eventLoggerRequester = boost::make_shared<requesters::CEventLogger>(m_databaseRequester);
         m_acquisitionRequester = boost::make_shared<requesters::CAcquisition>(m_databaseRequester, m_keywordRequester);
         m_ruleRequester = boost::make_shared<requesters::CRule>(m_databaseRequester);
         m_recipientRequester = boost::make_shared<requesters::CRecipient>(m_databaseRequester);
         m_serialPortRequester = boost::make_shared<requesters::CSerialPort>(m_databaseRequester);
      }


      boost::shared_ptr<ITransactionalProvider> CDataProvider::getTransactionalEngine()
      {
         if (m_databaseRequester->transactionSupport() && !m_databaseRequester->transactionIsAlreadyCreated())
            return m_databaseRequester;
         return boost::shared_ptr<ITransactionalProvider>();
      }

      boost::shared_ptr<IPluginRequester> CDataProvider::getPluginRequester()
      {
         return m_pluginRequester;
      }

      boost::shared_ptr<IConfigurationRequester> CDataProvider::getConfigurationRequester()
      {
         return m_configurationRequester;
      }

      boost::shared_ptr<IDeviceRequester> CDataProvider::getDeviceRequester()
      {
         return m_deviceRequester;
      }

      boost::shared_ptr<IKeywordRequester> CDataProvider::getKeywordRequester()
      {
         return m_keywordRequester;
      }

      boost::shared_ptr<IPageRequester> CDataProvider::getPageRequester()
      {
         return m_pageRequester;
      }

      boost::shared_ptr<IWidgetRequester> CDataProvider::getWidgetRequester()
      {
         return m_widgetRequester;
      }

      boost::shared_ptr<IPluginEventLoggerRequester> CDataProvider::getPluginEventLoggerRequester()
      {
         return m_pluginEventLoggerRequester;
      }

      boost::shared_ptr<IEventLoggerRequester> CDataProvider::getEventLoggerRequester()
      {
         return m_eventLoggerRequester;
      }

      boost::shared_ptr<IAcquisitionRequester> CDataProvider::getAcquisitionRequester()
      {
         return m_acquisitionRequester;
      }

      boost::shared_ptr<IRuleRequester> CDataProvider::getRuleRequester()
      {
         return m_ruleRequester;
      }

      boost::shared_ptr<IRecipientRequester> CDataProvider::getRecipientRequester()
      {
         return m_recipientRequester;
      }

      boost::shared_ptr<IDatabaseRequester> CDataProvider::getDatabaseRequester()
      {
         return m_databaseRequester;
      }

      boost::shared_ptr<ISerialPortRequester> CDataProvider::getSerialPortRequester()
      {
         return m_serialPortRequester;
      }
   } //namespace common
} //namespace database
