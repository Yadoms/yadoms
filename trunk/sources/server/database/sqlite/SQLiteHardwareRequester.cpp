#include "SQLiteHardwareRequester.h"
#include "tools/Exceptions/NotImplementedException.h"
#include "SQLiteDatabaseProvider.h"

CSQLiteHardwareRequester::CSQLiteHardwareRequester(CSQLiteDatabaseProvider * pDatabaseHandler)
   :m_pDatabaseHandler(pDatabaseHandler)
{
}

CSQLiteHardwareRequester::~CSQLiteHardwareRequester()
{
}

// IHardwareRequester implementation
bool CSQLiteHardwareRequester::createHardware(boost::scoped_ptr<CHardware> hardwareToRemove)
{
   throw CNotImplementedException();
}

boost::scoped_ptr<CHardware> CSQLiteHardwareRequester::getHardware(const int hardwareId)
{
   throw CNotImplementedException();
}

std::list<boost::scoped_ptr<CHardware>> CSQLiteHardwareRequester::getHardwares()
{
   throw CNotImplementedException();
}

bool CSQLiteHardwareRequester::updateHardware(boost::scoped_ptr<CHardware> hardwareToUpdate)
{
   throw CNotImplementedException();
}

bool CSQLiteHardwareRequester::removeHardware(boost::scoped_ptr<CHardware> hardwareToRemove)
{
   throw CNotImplementedException();
}
// [END] IHardwareRequester implementation




