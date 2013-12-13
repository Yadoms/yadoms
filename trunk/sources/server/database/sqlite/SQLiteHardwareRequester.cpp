#include "stdafx.h"
#include "SQLiteHardwareRequester.h"
#include "tools/Exceptions/NotImplementedException.h"
#include "SQLiteDataProvider.h"
#include "adapters/SingleValueAdapter.hpp"
#include "adapters/HardwareAdapter.h"

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
   throw CNotImplementedException();
}

boost::shared_ptr<CHardware> CSQLiteHardwareRequester::getHardware(int hardwareId)
{
   CHardwareAdapter adapter;
   std::ostringstream os;
   os << "SELECT * FROM " << m_tableName << " WHERE id=\"" << hardwareId << "\"";
   m_databaseRequester->queryEntities<boost::shared_ptr<CHardware> >(&adapter, os.str());
   if (adapter.getResults().empty())
   {
      // Hardware not found
      throw std::invalid_argument(CStringExtension::format("Hardware Id %d not found in database", hardwareId));
   }
   return adapter.getResults().at(0);
}

std::vector<boost::shared_ptr<CHardware> > CSQLiteHardwareRequester::getHardwares()
{
   CHardwareAdapter adapter;
   m_databaseRequester->queryEntities<boost::shared_ptr<CHardware> >(&adapter, "SELECT * FROM Hardware");
   return adapter.getResults();
}

//test
std::vector<std::string> CSQLiteHardwareRequester::getHardwareNameList()
{
   CSingleValueAdapter<std::string> adapter;
   m_databaseRequester->queryEntities<std::string>(&adapter, "SELECT Name FROM " + m_tableName + " WHERE id=%d ORDER BY Name", 2);
   return adapter.getResults();
}

bool CSQLiteHardwareRequester::updateHardware(boost::shared_ptr<CHardware> hardware)
{
   throw CNotImplementedException();
}

bool CSQLiteHardwareRequester::removeHardware(int hardwareId)
{
   throw CNotImplementedException();
}

void CSQLiteHardwareRequester::enableInstance(int hardwareId, bool enable)
{
   throw CNotImplementedException();
}
// [END] IHardwareRequester implementation




