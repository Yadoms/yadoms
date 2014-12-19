#include "stdafx.h"
#include "Job.h"
#include "database/sqlite/SQLiteDataProvider.h"
#include "database/sqlite/adapters/SQLiteDatabaseAdapters.h"
#include "database/sqlite/SQLiteDatabaseTables.h"
#include "database/sqlite/Query.h"


namespace database { namespace sqlite { namespace requesters { 

   CJob::CJob(boost::shared_ptr<CSQLiteRequester> & databaseRequester)
      :m_databaseRequester(databaseRequester)
   {
   }

   CJob::~CJob()
   {
   }

   // IJobRequester implementation

   std::vector<boost::shared_ptr<entities::CJob> >  CJob::getJobs() const
   {
      CQuery qSelect;
      qSelect. Select().
         From(CJobTable::getTableName());

      adapters::CJobAdapter adapter;
      m_databaseRequester->queryEntities<boost::shared_ptr<entities::CJob> >(&adapter, qSelect);
      return adapter.getResults();
   }

   // [END] IJobRequester implementation

} //namespace requesters
} //namespace sqlite
} //namespace database 

