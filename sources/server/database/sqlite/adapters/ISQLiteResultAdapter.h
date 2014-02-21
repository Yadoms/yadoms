#pragma once

#include <sqlite3.h>

namespace server { 
namespace database { 
namespace sqlite { 
namespace adapters { 


   //--------------------------------------------------------------
   /// \Brief		Interface for result adapters
   ///				The aim is to provide method to adapt sqlite results
   ///				to a list of entities
   //--------------------------------------------------------------
   template<class T>
   class ISQLiteResultAdapter
   {
   public:
      //--------------------------------------------------------------
      /// \Brief		   Adapt a SQlite results (single row) to entity
      /// \param [in]	column         Number of columns in results
      /// \param [in]	columnName     The column values
      /// \param [in]	columnName     The column names
      /// \return       true if adapt is realized with success, else false
      //--------------------------------------------------------------
      virtual bool adapt(sqlite3_stmt * pStatement) =0;
   
      //--------------------------------------------------------------
      /// \Brief		   Get the full results (vector of entities)
      //--------------------------------------------------------------   
      virtual std::vector<T> getResults() = 0;
   };

} //namespace adapters
} //namespace sqlite
} //namespace database 
} //namespace server
