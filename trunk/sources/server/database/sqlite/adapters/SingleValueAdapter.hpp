#pragma once

#include "SQLite3Extension.hpp"


namespace database { 
namespace sqlite { 
namespace adapters { 


   //--------------------------------------------------------------
   ///\Brief		Class which adapt in single column resultset
   ///\template   TValue : the type of value
   ///\example    CSingleValueAdapter<int> will provide std::vector<int>
   //--------------------------------------------------------------
   template<class TValue>
   class CSingleValueAdapter: public ISQLiteResultAdapter<TValue>
   {
   public:
      //--------------------------------------------------------------
      /// \Brief		Constructor
      //--------------------------------------------------------------
      CSingleValueAdapter()
      {
      }
   
      //--------------------------------------------------------------
      /// \Brief		Destructor
      //--------------------------------------------------------------
      virtual ~CSingleValueAdapter()
      {
      }
   
      // ISQLiteResultAdapter implementation
      bool adapt(sqlite3_stmt * pStatement)
      {
         int nCols = sqlite3_column_count(pStatement);
         if (nCols == 1) 
         {
            while (sqlite3_step(pStatement) == SQLITE_ROW) 
            {
               m_results.push_back(CSQLite3Extension::extractData<TValue>(pStatement, 0));
            }
            return true;
         }
         return false;
      }

      std::vector<TValue> getResults()
      {
         return m_results;
      }
      // [END] ISQLiteResultAdapter implementation

   private:
      //--------------------------------------------------------------
      /// \Brief		Contains the list of results entities
      //--------------------------------------------------------------
       std::vector<TValue> m_results;
   };

   //--------------------------------------------------------------
   ///\Brief		Class which adapt in single column resultset
   ///\template   TValue : the type of value
   ///\example    CSingleValueAdapter<int> will provide std::vector<int>
   //--------------------------------------------------------------
   template<class TValue>
   class CSingleValueAdapterWithContainer: public ISQLiteResultAdapter<TValue>
   {
   public:
      //--------------------------------------------------------------
      /// \Brief		Constructor
      //--------------------------------------------------------------
      CSingleValueAdapterWithContainer(std::vector<TValue> & c)
         :m_results(c)
      {
      }
   
      //--------------------------------------------------------------
      /// \Brief		Destructor
      //--------------------------------------------------------------
      virtual ~CSingleValueAdapterWithContainer()
      {
      }
   
      // ISQLiteResultAdapter implementation
      bool adapt(sqlite3_stmt * pStatement)
      {
         int nCols = sqlite3_column_count(pStatement);
         if (nCols == 1) 
         {
            while (sqlite3_step(pStatement) == SQLITE_ROW) 
            {
               m_results.push_back(CSQLite3Extension::extractData<TValue>(pStatement, 0));
            }
            return true;
         }
         return false;
      }

      std::vector<TValue> getResults()
      {
         return m_results;
      }
      // [END] ISQLiteResultAdapter implementation

   private:
      //--------------------------------------------------------------
      /// \Brief		Contains the list of results entities
      //--------------------------------------------------------------
       std::vector<TValue> & m_results;
   };




} //namespace adapters
} //namespace sqlite
} //namespace database 

