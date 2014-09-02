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
   class CRawValueAdapter: public ISQLiteResultAdapter<std::string>
   {
   public:
      //--------------------------------------------------------------
      /// \Brief		Constructor
      //--------------------------------------------------------------
      CRawValueAdapter()
      {
      }
   
      //--------------------------------------------------------------
      /// \Brief		Destructor
      //--------------------------------------------------------------
      virtual ~CRawValueAdapter()
      {
      }
   
      // ISQLiteResultAdapter implementation
      bool adapt(sqlite3_stmt * pStatement)
      {
         int nCols = sqlite3_column_count(pStatement);
         if (nCols == 1) 
         {
            m_internalValue = "[";
            while (sqlite3_step(pStatement) == SQLITE_ROW) 
            {
               m_internalValue += "[" + CSQLite3Extension::extractData<std::string>(pStatement, 0) + "],";
            }
            m_internalValue[m_internalValue.size() - 1] = ']';
            return true;
         }
         return false;
      }

      std::vector<std::string> getResults()
      {
         return boost::assign::list_of(m_internalValue);
      }   
      
      std::string getRawResults()
      {
         return m_internalValue;
      }
      // [END] ISQLiteResultAdapter implementation

   private:
      std::string m_internalValue;

   };


} //namespace adapters
} //namespace sqlite
} //namespace database 

