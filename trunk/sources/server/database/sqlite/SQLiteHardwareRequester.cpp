#include "SQLiteHardwareRequester.h"
#include "tools/Exceptions/NotImplementedException.h"
#include "SQLiteDataProvider.h"
#include "boost/lexical_cast.hpp"
#include "adapters/SingleValueAdapter.hpp"

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
   CHardwareAdapter adapter;
   m_pDatabaseHandler->queryEntities<CHardware>(&adapter, "SELECT * FROM Hardware");
   return adapter.getResults();
}

//test
std::vector<std::string > CSQLiteHardwareRequester::getHardwareNameList()
{
   CSingleValueAdapter<std::string> adapter;
   m_pDatabaseHandler->queryEntities<std::string>(&adapter, "SELECT Name FROM Hardware WHERE id=%d ORDER BY Name", 2);
   return adapter.getResults();
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




