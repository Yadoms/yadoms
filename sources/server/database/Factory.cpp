#include "stdafx.h"
#include "Factory.h"
#include <shared/ServiceLocator.h>
#include "startupOptions/IStartupOptions.h"
#include "common/DataProvider.h"
#include "sqlite/SQLiteRequester.h"

#ifndef PGSQL_NOT_FOUND
#  include "pgsql/PgsqlRequester.h"
#  include "pgsql/PgsqlLibrary.h"
#endif

namespace database
{
   boost::shared_ptr<IDataProvider> CFactory::create(const IPathProvider& pathProvider)
   {
      auto databaseEngine = createEngine(pathProvider);
      return boost::make_shared<common::CDataProvider>(databaseEngine);
   }

   boost::shared_ptr<IDatabaseRequester> CFactory::createEngine(const IPathProvider& pathProvider)
   {
      const auto startupOptions = shared::CServiceLocator::instance().get<startupOptions::IStartupOptions>();
      const auto dbEngine = startupOptions->getDatabaseEngine();

      switch (dbEngine)
      {
      case startupOptions::EDatabaseEngine::kSqliteValue:
         {
            return boost::make_shared<sqlite::CSQLiteRequester>(pathProvider.databaseSqliteFile().string(),
                                                                pathProvider.databaseSqliteBackupFile().string());
         }
#ifndef PGSQL_NOT_FOUND
      case startupOptions::EDatabaseEngine::kPostgresqlValue:
         {
            // PostgreSql library is dynamically loaded to avoid depandancy on it as it us rarely used (and depandancy to MSVCRT under Windows)
            const auto pqsqlLibrary = boost::make_shared<pgsql::CPgsqlLibrary>();
            return boost::make_shared<pgsql::CPgsqlRequester>(pqsqlLibrary);
         }
#endif
      default:
         throw CDatabaseException("Unsupported database engine");
      }
   }
} //namespace database 


