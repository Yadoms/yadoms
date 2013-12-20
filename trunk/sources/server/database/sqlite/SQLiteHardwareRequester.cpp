#include "stdafx.h"
#include "SQLiteHardwareRequester.h"
#include "tools/Exceptions/NotImplementedException.hpp"
#include "SQLiteDataProvider.h"
#include "adapters/SingleValueAdapter.hpp"
#include "adapters/SQLiteDatabaseAdapters.h"
#include "tools/Exceptions/EmptyResultException.hpp"
#include "tools/Exceptions/InvalidParameterException.hpp"

#include "database/sqlite/SQLiteDatabaseTables.h"

const std::string CSQLiteHardwareRequester::m_tableName("Hardware");

CSQLiteHardwareRequester::CSQLiteHardwareRequester(const CSQLiteDataProvider & databaseHandler, boost::shared_ptr<CSQLiteRequester> & databaseRequester)
   :m_databaseHandler(databaseHandler), m_databaseRequester(databaseRequester)
{
}

CSQLiteHardwareRequester::~CSQLiteHardwareRequester()
{
}

// IHardwareRequester implementation
int CSQLiteHardwareRequester::addHardware(boost::shared_ptr<CHardware> newHardware)
{
   std::ostringstream os;
   os << "INSERT INTO " << m_tableName << " (name, pluginName, configuration, enabled) " <<
      " VALUES('" <<  newHardware->getName() << "','" << 
                     newHardware->getPluginName() << "','" << 
                     newHardware->getConfiguration() << "'," << 
                     newHardware->getEnabled() << ")";

   if(m_databaseRequester->queryStatement(os.str()) <= 0)
      throw new CEmptyResultException("No lines affected");

   std::ostringstream os2;
   os2 <<  "SELECT id FROM " << m_tableName << " WHERE name='" <<  newHardware->getName() << "' AND pluginName='" << newHardware->getPluginName() << "' ORDER BY id DESC";

   CSingleValueAdapter<int> adapter;
   m_databaseRequester->queryEntities<int>(&adapter, os2.str());
   if(adapter.getResults().size() >= 1)
      return adapter.getResults()[0];
   else
      throw new CEmptyResultException("Cannot retrieve inserted Hardware");
}

boost::shared_ptr<CHardware> CSQLiteHardwareRequester::getHardware(int hardwareId)
{
   CHardwareAdapter adapter;
   std::ostringstream os;
   os << "SELECT * FROM " << m_tableName << " WHERE id=" << hardwareId << " AND deleted=0";
   m_databaseRequester->queryEntities<boost::shared_ptr<CHardware> >(&adapter, os.str());
   if (adapter.getResults().empty())
   {
      // Hardware not found
      throw CInvalidParameterException(CStringExtension::format("Hardware Id %d not found in database", hardwareId));
   }
   return adapter.getResults().at(0);
}

std::vector<boost::shared_ptr<CHardware> > CSQLiteHardwareRequester::getHardwares(bool evenDeleted)
{
   CHardwareAdapter adapter;
   std::string request = "SELECT * FROM Hardware";
   if (!evenDeleted)
      request += " WHERE deleted=0";
   m_databaseRequester->queryEntities<boost::shared_ptr<CHardware> >(&adapter, request);
   return adapter.getResults();
}

//test
std::vector<std::string> CSQLiteHardwareRequester::getHardwareNameList()
{
   CSingleValueAdapter<std::string> adapter;
   m_databaseRequester->queryEntities<std::string>(&adapter, "SELECT Name FROM " + m_tableName + " WHERE id=%d AND deleted=0 ORDER BY Name", 2);
   return adapter.getResults();
}

void CSQLiteHardwareRequester::updateHardwareConfiguration(int hardwareId, const std::string& newConfiguration)
{
   std::ostringstream os;
   os << "UPDATE " << m_tableName << " SET configuration = '" << newConfiguration << "' WHERE id=" << hardwareId;

   if(m_databaseRequester->queryStatement(os.str()) <= 0)
      throw new CEmptyResultException("No lines affected");
}

void CSQLiteHardwareRequester::removeHardware(int hardwareId)
{
   std::ostringstream os;
   os << "UPDATE " << m_tableName << " SET deleted = 1 WHERE id=" << hardwareId;

   if(m_databaseRequester->queryStatement(os.str()) <= 0)
      throw new CEmptyResultException("No lines affected");
}

void CSQLiteHardwareRequester::enableInstance(int hardwareId, bool enable)
{
   std::ostringstream os;
   os << "UPDATE " << m_tableName << " SET enabled = " << (enable?1:0) << " WHERE id=" << hardwareId;

   if(m_databaseRequester->queryStatement(os.str()) <= 0)
      throw new CEmptyResultException("No lines affected");
}
// [END] IHardwareRequester implementation




