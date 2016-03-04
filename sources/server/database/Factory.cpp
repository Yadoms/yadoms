#include "stdafx.h"
#include "Factory.h"
#include <shared/Log.h>
#include <shared/ServiceLocator.h>
#include "startupOptions/IStartupOptions.h"
#include "common/DataProvider.h"
#include "DatabaseException.hpp"
#include "sqlite/SQLiteRequester.h"
#include "pgsql/PgsqlRequester.h"

namespace database { 

   boost::shared_ptr<database::IDataProvider> CFactory::create()
   {
      boost::shared_ptr<IDatabaseRequester> databaseEngine = createEngine();
      return boost::shared_ptr<database::IDataProvider>(new database::common::CDataProvider(databaseEngine));
   }

   boost::shared_ptr<IDatabaseRequester> CFactory::createEngine()
   {
      boost::shared_ptr<startupOptions::IStartupOptions> startupOptions = shared::CServiceLocator::instance().get<startupOptions::IStartupOptions>();

      startupOptions::EDatabaseEngine dbEngine = startupOptions->getDatabaseEngine();

      switch (dbEngine)
      {
      case startupOptions::EDatabaseEngine::kSqliteValue:
         return boost::shared_ptr<IDatabaseRequester>(new sqlite::CSQLiteRequester(startupOptions->getDatabaseSqliteFile()));
         break;
      case startupOptions::EDatabaseEngine::kPostgresqlValue:
         return boost::shared_ptr<IDatabaseRequester>(new pgsql::CPgsqlRequester(startupOptions->getDatabasePostgresqlHost(), startupOptions->getDatabasePostgresqlPort(), startupOptions->getDatabasePostgresqlDbName(), startupOptions->getDatabasePostgresqlLogin(), startupOptions->getDatabasePostgresqlPassword()));
         break;
      default:
         throw CDatabaseException("Unsupported database engine");
      }
   }

} //namespace database 

