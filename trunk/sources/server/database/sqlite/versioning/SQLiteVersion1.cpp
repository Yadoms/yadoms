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
   //check that table Configuration exists
   const std::string sCheckForConfigurationTableExists = "SELECT count(*) FROM sqlite_master WHERE type='table' AND name='configuration'";
   int count = pRequester->queryCount(sCheckForConfigurationTableExists);

   


}
// [END] ISQLiteVersionUpgrade implementation