#include "stdafx.h"
#include "SQLiteVersion1.h"
#include "database/sqlite/Query.h"

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
   CQuery sCheckForConfigurationTableExists;

   sCheckForConfigurationTableExists.  selectCount().
                                       from("sqlite_master").
                                       where("type", CQUERY_OP_EQUAL, "table").
                                       and("name", CQUERY_OP_EQUAL, "configuration");
   int count = pRequester->queryCount(sCheckForConfigurationTableExists);

   


}
// [END] ISQLiteVersionUpgrade implementation