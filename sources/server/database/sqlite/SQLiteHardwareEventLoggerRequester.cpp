#include "stdafx.h"
#include "SQLiteHardwareEventLoggerRequester.h"
#include <shared/Exceptions/NotImplementedException.hpp>
#include "SQLiteDataProvider.h"
#include "adapters/SingleValueAdapter.hpp"
#include "adapters/SQLiteDatabaseAdapters.h"
#include <shared/Exceptions/EmptyResultException.hpp>
#include "SQLiteDatabaseTables.h"
#include "Query.h"


CSQLiteHardwareEventLoggerRequester::CSQLiteHardwareEventLoggerRequester(const CSQLiteDataProvider & databaseHandler, boost::shared_ptr<CSQLiteRequester> & databaseRequester)
   :m_databaseHandler(databaseHandler), m_databaseRequester(databaseRequester)
{
}

CSQLiteHardwareEventLoggerRequester::~CSQLiteHardwareEventLoggerRequester()
{
}

// IHardwareEventLoggerRequester implementation

int CSQLiteHardwareEventLoggerRequester::addEvent(const std::string & pluginName, const std::string & pluginVersion, const IHardwarePluginInformation::EReleaseType pluginReleaseType, const CHardwareEventLogger::EEventType eventType, const std::string & message /*= CStringExtension::EmptyString*/)
{
   boost::posix_time::ptime insertDate = boost::posix_time::second_clock::universal_time();

   CQuery qInsert;
   qInsert. InsertInto(CHardwareEventLoggerTable::getTableName(), CHardwareEventLoggerTable::getPluginNameColumnName(), CHardwareEventLoggerTable::getPluginVersionColumnName(), CHardwareEventLoggerTable::getPluginReleaseColumnName(), CHardwareEventLoggerTable::getEventTypeColumnName(), CHardwareEventLoggerTable::getMessageColumnName(), CHardwareEventLoggerTable::getEventDateColumnName()).
      Values(pluginName, pluginVersion, pluginReleaseType, eventType, message, insertDate);
   
   if(m_databaseRequester->queryStatement(qInsert) <= 0)
      throw CEmptyResultException("No lines affected");

   CQuery qSelect;
   qSelect. Select(CHardwareEventLoggerTable::getIdColumnName()).
      From(CHardwareEventLoggerTable::getTableName()).
      Where(CHardwareEventLoggerTable::getPluginNameColumnName(), CQUERY_OP_EQUAL, pluginName).
      And(CHardwareEventLoggerTable::getPluginVersionColumnName(), CQUERY_OP_EQUAL, pluginVersion).
      And(CHardwareEventLoggerTable::getPluginReleaseColumnName(), CQUERY_OP_EQUAL, pluginReleaseType).
      And(CHardwareEventLoggerTable::getEventTypeColumnName(), CQUERY_OP_EQUAL, eventType).
      And(CHardwareEventLoggerTable::getEventDateColumnName(), CQUERY_OP_EQUAL, insertDate).
      OrderBy(CHardwareEventLoggerTable::getIdColumnName(), CQUERY_ORDER_DESC);

   CSingleValueAdapter<int> adapter;
   m_databaseRequester->queryEntities<int>(&adapter, qSelect);
   if(adapter.getResults().size() >= 1)
      return adapter.getResults()[0];
   else
      throw CEmptyResultException("Cannot retrieve inserted Hardware");      
}

int CSQLiteHardwareEventLoggerRequester::addEvent(const CHardwareEventLogger & hardwareLogEntry)
{
   return addEvent(hardwareLogEntry.getPluginName(), hardwareLogEntry.getPluginVersion(), hardwareLogEntry.getPluginRelease(), hardwareLogEntry.getEventType(), hardwareLogEntry.getMessage());
}

std::vector<boost::shared_ptr<CHardwareEventLogger> > CSQLiteHardwareEventLoggerRequester::getHardwareEvents(const std::string & pluginName, const std::string & pluginVersion, const IHardwarePluginInformation::EReleaseType pluginReleaseType)
{
   CQuery qSelect;
   qSelect. Select(CHardwareEventLoggerTable::getIdColumnName()).
      From(CHardwareEventLoggerTable::getTableName()).
      Where(CHardwareEventLoggerTable::getPluginNameColumnName(), CQUERY_OP_EQUAL, pluginName).
      And(CHardwareEventLoggerTable::getPluginVersionColumnName(), CQUERY_OP_EQUAL, pluginVersion).
      And(CHardwareEventLoggerTable::getPluginReleaseColumnName(), CQUERY_OP_EQUAL, pluginReleaseType).
      OrderBy(CHardwareEventLoggerTable::getEventDateColumnName(), CQUERY_ORDER_DESC);

   CHardwareEventLoggerAdapter adapter;
   m_databaseRequester->queryEntities<boost::shared_ptr<CHardwareEventLogger> >(&adapter, qSelect);
   return adapter.getResults();
}


std::vector<boost::shared_ptr<CHardwareEventLogger> > CSQLiteHardwareEventLoggerRequester::getHardwareEvents(const std::string & pluginName, const std::string & pluginVersion, const IHardwarePluginInformation::EReleaseType pluginReleaseType, const boost::posix_time::ptime & fromDate)
{
   CQuery qSelect;
   qSelect. Select(CHardwareEventLoggerTable::getIdColumnName()).
      From(CHardwareEventLoggerTable::getTableName()).
      Where(CHardwareEventLoggerTable::getPluginNameColumnName(), CQUERY_OP_EQUAL, pluginName).
      And(CHardwareEventLoggerTable::getPluginVersionColumnName(), CQUERY_OP_EQUAL, pluginVersion).
      And(CHardwareEventLoggerTable::getPluginReleaseColumnName(), CQUERY_OP_EQUAL, pluginReleaseType).
      And(CHardwareEventLoggerTable::getEventDateColumnName(), CQUERY_OP_SUP, fromDate).
      OrderBy(CHardwareEventLoggerTable::getEventDateColumnName(), CQUERY_ORDER_DESC);

   CHardwareEventLoggerAdapter adapter;
   m_databaseRequester->queryEntities<boost::shared_ptr<CHardwareEventLogger> >(&adapter, qSelect);
   return adapter.getResults();
}


// [END] IHardwareEventLoggerRequester implementation




