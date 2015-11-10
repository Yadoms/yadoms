#pragma once

#include <Poco/Util/TimerTask.h>
#include "database/sqlite/SQLiteDataProvider.h"

namespace database { 
namespace sqlite { 

   class CSQLiteSummaryDataTask : public Poco::Util::TimerTask
   {
   public:
      //--------------------------------------------------------------
      /// \brief	Constructor
      /// \param [in]	databaseProvider: the database provider
      //--------------------------------------------------------------
      CSQLiteSummaryDataTask(CSQLiteDataProvider * databaseProvider);

      //--------------------------------------------------------------
      /// \brief	Destructor
      //--------------------------------------------------------------
      virtual ~CSQLiteSummaryDataTask();

      // BEGIN Poco::Util::TimerTask implementation 
      virtual void run();
      // END Poco::Util::TimerTask implementation 

   private:
      void executeFirstRunPass();
      void executeCommonPass();
      

      //--------------------------------------------------------------
      /// \Brief		   Pointer to SQLite database provider (real pointer because this object is only instanciated by CSQLiteDataProvider)
      //--------------------------------------------------------------
      CSQLiteDataProvider * m_databaseProvider;

      //--------------------------------------------------------------
      /// \Brief		   Keep the last date time used to compute summary day data
      //--------------------------------------------------------------
      bool m_firstRun;
   };

} //namespace sqlite
} //namespace database 

