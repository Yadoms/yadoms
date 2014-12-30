#include "stdafx.h"
#include "GenericAdapter.h"
#include <shared/Log.h>
#include "SQLite3Extension.hpp"


namespace database { 
namespace sqlite { 
namespace adapters { 


   CGenericAdapter::CGenericAdapter()
   {
   }

   CGenericAdapter::~CGenericAdapter()
   {
   }

   bool CGenericAdapter::adapt(sqlite3_stmt * pStatement)
   {
      int nCols = sqlite3_column_count(pStatement);
      if (nCols) 
      {
         std::vector<std::string> cols;
         for (int nCol = 0; nCol < nCols; nCol++) 
            cols.push_back(std::string(sqlite3_column_name(pStatement, nCol)));

         while (sqlite3_step(pStatement) == SQLITE_ROW) 
         {
            std::map<std::string, std::string> newRow;
            for (int nCol = 0; nCol < nCols;nCol++) 
               newRow.insert(std::make_pair(cols[nCol], CSQLite3Extension::extractData<std::string>(pStatement, nCol) ));
            m_results.push_back(newRow);
         }
         return true;
      }
      return false;
   }


   std::vector<const std::map<std::string, std::string> > CGenericAdapter::getResults() const
   { 
      return m_results; 
   }

} //namespace adapters
} //namespace sqlite
} //namespace database 

