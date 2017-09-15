#include "stdafx.h"
#include "Factory.h"
#include "startupOptions/IStartupOptions.h"
#include "common/DataProvider.h"
#include "sqlite/SQLiteRequester.h"

#ifndef PGSQL_NOT_FOUND
#  include "pgsql/PgsqlRequester.h"
#  include "pgsql/PgsqlLibrary.h"
#endif

namespace database
{
   CFactory::CFactory(const IPathProvider& pathProvider,
                      boost::shared_ptr<const startupOptions::IStartupOptions> startupOptions)
      : m_pathProvider(pathProvider),
        m_startupOptions(startupOptions)
   {
   }

   CFactory::~CFactory()
   {
   }

   boost::shared_ptr<IDataProvider> CFactory::createDataProvider() const
   {
      return boost::make_shared<common::CDataProvider>(createEngine());
   }

   boost::shared_ptr<IDatabaseRequester> CFactory::createEngine() const
   {
      const auto dbEngine = m_startupOptions->getDatabaseEngine();

      switch (dbEngine)
      {
      case startupOptions::EDatabaseEngine::kSqliteValue:
         {
            return boost::make_shared<sqlite::CSQLiteRequester>(m_pathProvider.databaseSqliteFile().string(),
                                                                m_pathProvider.databaseSqliteBackupFile().string());
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


