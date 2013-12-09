#include "stdafx.h"
#include "SQLiteVersion1.h"

CSQLiteVersion1::CSQLiteVersion1()
{
}

CSQLiteVersion1::~CSQLiteVersion1()
{
}

// ISQLiteVersionUpgrade implementation
void CSQLiteVersion1::checkForUpgrade(const boost::shared_ptr<CSQLiteRequester> & pRequester)
{

   const std::string sCheckForConfigurationTableExists = "SELECT count(*) FROM sqlite_master WHERE type='table' AND name='configuration'";
   CSQLiteRequester::QueryRow  result = pRequester->querySingleLine(sCheckForConfigurationTableExists);


}
// [END] ISQLiteVersionUpgrade implementation