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
bool CSQLiteHardwareRequester::createHardware(boost::shared_ptr<CHardware> hardwareToCreate)
{
   throw CNotImplementedException();
}

boost::shared_ptr<CHardware> CSQLiteHardwareRequester::getHardware(const int hardwareId)
{
   CHardwareAdapter adapter;
   std::ostringstream os;
   os << "SELECT * FROM Hardware WHERE id=\"" << hardwareId << "\"";
   m_databaseRequester->queryEntities<boost::shared_ptr<CHardware> >(&adapter, os.str());
   // TODO : gérer exception si non trouvé
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
   m_databaseRequester->queryEntities<std::string>(&adapter, "SELECT Name FROM Hardware WHERE id=%d ORDER BY Name", 2);
   return adapter.getResults();
}

bool CSQLiteHardwareRequester::updateHardware(boost::shared_ptr<CHardware> hardwareToUpdate)
{
   throw CNotImplementedException();
}

bool CSQLiteHardwareRequester::removeHardware(boost::shared_ptr<CHardware> hardwareToRemove)
{
   throw CNotImplementedException();
}
// [END] IHardwareRequester implementation




