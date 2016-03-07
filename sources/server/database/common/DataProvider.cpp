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
#include <shared/exception/NotSupported.hpp>
#include "database/DatabaseException.hpp"

namespace database { namespace common {

   CDataProvider::CDataProvider(boost::shared_ptr<database::IDatabaseRequester> databaseRequester)
      :m_databaseRequester(databaseRequester)
   {
   }

   CDataProvider::~CDataProvider()
   {
      stopMaintenanceTasks();
      unload();
   }

   shared::versioning::CVersion CDataProvider::GetVersion()
   {
      //get the database version
      shared::versioning::CVersion currentVersion;

      try
      {
         if (m_databaseRequester->checkTableExists(CConfigurationTable::getTableName()))
         {
            CQuery qVersion = m_databaseRequester->newQuery();
            qVersion.Select(CConfigurationTable::getValueColumnName()).
               From(CConfigurationTable::getTableName()).
               Where(CConfigurationTable::getSectionColumnName(), CQUERY_OP_EQUAL, "Database").
               And(CConfigurationTable::getNameColumnName(), CQUERY_OP_EQUAL, "Version");

            adapters::CSingleValueAdapter<std::string> adapter;
            m_databaseRequester->queryEntities(&adapter, qVersion);
            std::vector<std::string> results = adapter.getResults();


            if (results.size() >= 1)
            {
               currentVersion = shared::versioning::CVersion(results[0]);
            }
         }
      }
      catch (std::exception & ex)
      {
         YADOMS_LOG(debug) << "Fail to get version of database : " << ex.what();
      }
      catch (...)
      {
         YADOMS_LOG(debug) << "Fail to get version of database : Unkonown exception";
      }

      return currentVersion;
   }

   // IDatabaseProvider implementation
   bool CDataProvider::load()
   {
      YADOMS_LOG(information) << "Load database";

      bool result = false;

      try
      {
         //initialize database engine
         m_databaseRequester->initialize();

         //check for update
         YADOMS_LOG(information) << "Check for database update...";

         versioning::CVersionUpgraderFactory::GetUpgrader()->checkForUpgrade(m_databaseRequester, GetVersion());

         //create entities requester (high level querier)
         loadRequesters();

         YADOMS_LOG(information) << "Load database with success";
         result = true;

         //database loaded, start maintenance task
         initializeMaintenanceTasks();               
      }
      catch (versioning::CVersionException & exc)
      {
         YADOMS_LOG(error) << "Fail to load database (upgrade error) : " << std::endl << exc.what();
         result = false;
      }
      catch (database::CDatabaseException &dbex)
      {
         YADOMS_LOG(error) << "Database exception while loading database" << std::endl << dbex.what();
         result = false;
      }
      catch (std::exception &ex)
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
      m_maintenanceTimer.schedule(m_maintenanceSummaryComputingTask, Poco::Timestamp());

      //then schedule it to be run each hour (+1 minute in order to give a small latency for plugins): 00h01, 01h01, 02h01, 03h01, 04h01....
      //working with Poco::DateTime because using Poco::Util::Timer class
      Poco::DateTime now;
      Poco::DateTime firstOccurence(now.year(), now.month(), now.day(), now.hour(), 1);
      Poco::Timespan oneHourOffset(0, 1, 0, 0, 0);
      firstOccurence += oneHourOffset;//+1hour
      Poco::Timespan timeToWaitBeforeFirstOccurrence = firstOccurence - now;

      YADOMS_LOG(information) << "Summary task will happens @" << firstOccurence.hour() << ":" << firstOccurence.minute();
      YADOMS_LOG(information) << "Next step in " << timeToWaitBeforeFirstOccurrence.minutes() << " minutes";

      //schedule the timer to launch task each hour +1min
      //Schedule : now and each hour (1000 * 3600)
      long msWait = (long)timeToWaitBeforeFirstOccurrence.totalMilliseconds(); //force cast because value is maximum 1hour = 1000*3600 which is less than "long" maximum value
      long msWaitPeriod = (long)oneHourOffset.totalMilliseconds();//force cast because value is 1 hour = 1000*3600 which is less than "long" maximum value

      m_maintenanceTimer.scheduleAtFixedRate(m_maintenanceSummaryComputingTask, msWait, msWaitPeriod);
   }

   void CDataProvider::initializePurgeTask()
   {
      //create the purge task
      m_maintenancePurgeTask = Poco::Util::TimerTask::Ptr(new CPurgeTask(getAcquisitionRequester(), m_databaseRequester));

      //schedule task now (at app start)
      m_maintenanceTimer.schedule(m_maintenancePurgeTask, Poco::Timestamp());

      //schedule task now (at app start)
      //working with Poco::DateTime because using Poco::Util::Timer class
      Poco::DateTime now;
      Poco::DateTime firstPurgeOccurence(now.year(), now.month(), now.day(), now.hour(), 15);
      Poco::Timespan oneDayOffset(24, 0, 0, 0, 0);
      firstPurgeOccurence += oneDayOffset;//+1day
      Poco::Timespan timeToWaitBeforeFirstPurgeOccurrence = firstPurgeOccurence - now;

      long msWait = (long)timeToWaitBeforeFirstPurgeOccurrence.totalMilliseconds(); //force cast because value is maximum 1hour = 1000*3600 which is less than "long" maximum value
      long msWaitPeriod = (long)oneDayOffset.totalMilliseconds();//force cast because value is 1 hour = 1000*3600 which is less than "long" maximum value

      m_maintenanceTimer.scheduleAtFixedRate(m_maintenancePurgeTask, msWait, msWaitPeriod);
   }



   void CDataProvider::unload()
   {
      //initialize database engine
      if(m_databaseRequester)
         m_databaseRequester->finalize();
   }

   void CDataProvider::stopMaintenanceTasks()
   {
      //cancel all tasks
      if (m_maintenanceSummaryComputingTask)
         m_maintenanceSummaryComputingTask->cancel();
      if (m_maintenancePurgeTask)
         m_maintenancePurgeTask->cancel();
      m_maintenanceTimer.cancel();

      m_maintenancePurgeTask.assign(NULL);
      m_maintenanceSummaryComputingTask.assign(NULL);
   }

   void CDataProvider::loadRequesters()
   {
      m_pluginRequester.reset(new requesters::CPlugin( m_databaseRequester));
      m_configurationRequester.reset(new requesters::CConfiguration( m_databaseRequester));
      m_deviceRequester.reset(new requesters::CDevice( m_databaseRequester));
		m_keywordRequester.reset(new requesters::CKeyword(m_databaseRequester, m_acquisitionRequester));
      m_pageRequester.reset(new requesters::CPage( m_databaseRequester));
      m_widgetRequester.reset(new requesters::CWidget( m_databaseRequester));
      m_pluginEventLoggerRequester.reset(new requesters::CPluginEventLogger( m_databaseRequester));
      m_eventLoggerRequester.reset(new requesters::CEventLogger( m_databaseRequester));
      m_acquisitionRequester.reset(new requesters::CAcquisition(m_databaseRequester, m_keywordRequester));
      m_ruleRequester.reset(new requesters::CRule(m_databaseRequester));
      m_recipientRequester.reset(new requesters::CRecipient(m_databaseRequester));
   }



   boost::shared_ptr<ITransactionalProvider> CDataProvider::getTransactionalEngine()
   {
      if (m_databaseRequester->transactionSupport() && !m_databaseRequester->transactionIsAlreadyCreated())
         return m_databaseRequester;
      return boost::shared_ptr<ITransactionalProvider>();
   }

} //namespace common
} //namespace database 

