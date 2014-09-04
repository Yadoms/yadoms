#pragma once

#include "SQLite3Extension.hpp"
#include <shared/exception/NotSupported.hpp>

namespace database { 
namespace sqlite { 
namespace adapters { 

      //--------------------------------------------------------------
   ///\Brief		Class which adapt in single string, using a json like ouptut [[data],[data],....]
   //--------------------------------------------------------------
   class CHighchartValueAdapter : public ISQLiteResultAdapter<std::string>
   {
   public:
      //--------------------------------------------------------------
      /// \Brief		Constructor
      //--------------------------------------------------------------
      CHighchartValueAdapter()
      {
      }
   
      //--------------------------------------------------------------
      /// \Brief		Destructor
      //--------------------------------------------------------------
      virtual ~CHighchartValueAdapter()
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
         //getRawResults should be used
         throw shared::exception::CNotSupported("CHighchartValueAdapter::getResults");
      }   
      // [END] ISQLiteResultAdapter implementation

      //--------------------------------------------------------------
      /// \Brief		Get the result (raw format)
      /// \return		The result (raw format) using a json like ouptut [[data],[data],....]
      //--------------------------------------------------------------
      std::string getRawResults()
      {
         return m_internalValue;
      }

   private:
      std::string m_internalValue;

   };


} //namespace adapters
} //namespace sqlite
} //namespace database 

