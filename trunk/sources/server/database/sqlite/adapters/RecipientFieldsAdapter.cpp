#include "stdafx.h"
#include "RecipientFieldsAdapter.h"
#include "sqlite3.h"
#include <shared/Log.h>
#include "SQLite3Extension.hpp"
#include "database/sqlite/SQLiteDatabaseTables.h"

namespace database {   namespace sqlite {  namespace adapters {

   CRecipientFieldsAdapter::CRecipientFieldsAdapter() 
   {
   }

   CRecipientFieldsAdapter::~CRecipientFieldsAdapter() 
   {
   }

   bool CRecipientFieldsAdapter::adapt(sqlite3_stmt * pStatement) 
   {
      int nCols = sqlite3_column_count(pStatement);
      if (nCols) 
      {
         std::vector<std::string> cols;
         for (int nCol = 0; nCol < nCols; nCol++) 
            cols.push_back(std::string(sqlite3_column_name(pStatement, nCol)));

         while (sqlite3_step(pStatement) == SQLITE_ROW) 
         {
            boost::shared_ptr<database::entities::CField> newEntity(new database::entities::CField());
            for (int nCol = 0; nCol < nCols; nCol++) 
            {
               if(boost::iequals(CFieldTable::getIdColumnName(), cols[nCol])) 
               {
                  if(sqlite3_column_type(pStatement, nCol) == SQLITE_NULL) 
                     newEntity-> Id  = 0 ;
                  else 
                     newEntity-> Id  = ( CSQLite3Extension::extractData< int  >(pStatement, nCol) );
               } 
               else if(boost::iequals(CFieldTable::getNameColumnName(), cols[nCol])) 
               {
                  if(sqlite3_column_type(pStatement, nCol) == SQLITE_NULL) 
                     newEntity-> Name  = "" ; 
                  else 
                     newEntity-> Name  = ( CSQLite3Extension::extractData< std::string  >(pStatement, nCol) );
               } 
               else if(boost::iequals(CFieldTable::getCategoryColumnName(), cols[nCol])) 
               {
                  if(sqlite3_column_type(pStatement, nCol) == SQLITE_NULL) 
                     newEntity-> Category  = "" ;
                  else 
                     newEntity-> Category  = ( CSQLite3Extension::extractData< std::string  >(pStatement, nCol) );
               } 
               else if(boost::iequals(CFieldTable::getVerificationRegexColumnName(), cols[nCol])) 
               {
                  if(sqlite3_column_type(pStatement, nCol) == SQLITE_NULL) 
                     newEntity-> VerificationRegex  = "" ; 
                  else 
                     newEntity-> VerificationRegex  = ( CSQLite3Extension::extractData< std::string  >(pStatement, nCol) );
               }       
               else if(boost::iequals(CRecipientFieldsTable::getValueColumnName(), cols[nCol])) 
               {
                  if(sqlite3_column_type(pStatement, nCol) == SQLITE_NULL) 
                     newEntity-> Value  = "" ; 
                  else 
                     newEntity-> Value  = ( CSQLite3Extension::extractData< std::string  >(pStatement, nCol) );
               } 
               else 
               {
                  shared::CLog::logStream()->warning() << "Unknown column Name= " << cols[nCol] << " Value=" << CSQLite3Extension::extractData<std::string>(pStatement, nCol);
               }
            }
            m_results.push_back(newEntity);
         }
         return true;
      }
      return false;
   }

} //namespace adapters
} //namespace sqlite
} //namespace database 

