#include "stdafx.h"
#include "SQLiteHardwareRequester.h"
#include "tools/Exceptions/NotImplementedException.h"
#include "SQLiteDataProvider.h"
#include "adapters/SingleValueAdapter.hpp"
#include "adapters/HardwareAdapter.h"

CSQLiteHardwareRequester::CSQLiteHardwareRequester(const CSQLiteDataProvider & databaseHandler, boost::shared_ptr<CSQLiteRequester> & databaseRequester)
   :m_databaseHandler(databaseHandler), m_databaseRequester(databaseRequester)
{
}

CSQLiteHardwareRequester::~CSQLiteHardwareRequester()
{
}

// IHardwareRequester implementation
bool CSQLiteHardwareRequester::createHardware(CHardware & hardwareToCreate)
{
   throw CNotImplementedException();
}

CHardware CSQLiteHardwareRequester::getHardware(const int hardwareId)
{
   throw CNotImplementedException();
}

CHardware CSQLiteHardwareRequester::getHardware(const std::string& hardwareName)
{
   CHardwareAdapter adapter;
   std::ostringstream os;
   os << "SELECT * FROM Hardware WHERE name=\"" << hardwareName << "\"";
   m_databaseRequester->queryEntities<CHardware>(&adapter, os.str());
   // TODO : gérer exception si non trouvé
   return adapter.getResults().at(0);
}

std::vector<CHardware> CSQLiteHardwareRequester::getHardwares()
{
   CHardwareAdapter adapter;
   m_databaseRequester->queryEntities<CHardware>(&adapter, "SELECT * FROM Hardware");
   return adapter.getResults();
}

//test
std::vector<std::string > CSQLiteHardwareRequester::getHardwareNameList()
{
   CSingleValueAdapter<std::string> adapter;
   m_databaseRequester->queryEntities<std::string>(&adapter, "SELECT Name FROM Hardware WHERE id=%d ORDER BY Name", 2);
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




