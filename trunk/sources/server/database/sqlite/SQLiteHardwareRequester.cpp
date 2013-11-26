#include "SQLiteHardwareRequester.h"
#include "tools/Exceptions/NotImplementedException.h"
#include "SQLiteDataProvider.h"
#include "boost/lexical_cast.hpp"

CSQLiteHardwareRequester::CSQLiteHardwareRequester(CSQLiteDataProvider * pDatabaseHandler)
   :m_pDatabaseHandler(pDatabaseHandler)
{
}

CSQLiteHardwareRequester::~CSQLiteHardwareRequester()
{
}

// IHardwareRequester implementation
bool CSQLiteHardwareRequester::createHardware(CHardware & hardwareToRemove)
{
   throw CNotImplementedException();
}

CHardware CSQLiteHardwareRequester::getHardware(const int hardwareId)
{
   throw CNotImplementedException();
}

std::vector<CHardware> CSQLiteHardwareRequester::getHardwares()
{
   CSQLiteDataProvider::QueryResults a = getHardwareNameList();

   CHardwareAdapter adapter;
   m_pDatabaseHandler->queryEntities<CHardware>(&adapter, "SELECT * FROM Hardware");

   return adapter.getResults();
}

//test
std::vector<std::map<std::string, std::string> > CSQLiteHardwareRequester::getHardwareNameList()
{
   return m_pDatabaseHandler->query("SELECT Name FROM Hardware WHERE id=%d ORDER BY Name", 2);
}

bool CSQLiteHardwareRequester::updateHardware(CHardware & hardwareToUpdate)
{
   throw CNotImplementedException();
}

bool CSQLiteHardwareRequester::removeHardware(CHardware & hardwareToRemove)
{
   throw CNotImplementedException();
}
// [END] IHardwareRequester implementation




