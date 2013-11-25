#include "SQLiteHardwareRequester.h"
#include "tools/Exceptions/NotImplementedException.h"
#include "SQLiteDatabaseProvider.h"
#include "boost/lexical_cast.hpp"

CSQLiteHardwareRequester::CSQLiteHardwareRequester(CSQLiteDatabaseProvider * pDatabaseHandler)
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
   std::vector<CHardware> result;

   CSQLiteDatabaseProvider::QueryResults::iterator i;
   
   CSQLiteDatabaseProvider::QueryResults res = m_pDatabaseHandler->query("SELECT * FROM Hardware");

   //on parcours chaque ligne du résultat
   for(i=res.begin(); i!=res.end(); i++)
   {
      CHardware newHardware;
      newHardware.setId( boost::lexical_cast<int>((*i)[0]) );
      newHardware.setName( (*i)[1] );
      newHardware.setPluginName( (*i)[2] );
      result.push_back(newHardware);
   }

   return result;
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




