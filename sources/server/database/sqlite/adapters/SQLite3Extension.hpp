#pragma once

#include "sqlite3.h"
#include <shared/StringExtension.h>
#include <shared/plugin/information/IInformation.h>
#include "database/entities/Entities.h"


namespace database { 
namespace sqlite { 
namespace adapters { 


   //--------------------------------------------------------------
   ///\brief   Class (static) used to extend sqlite3 functions.
   ///         It provides a method to handle utf8 in std::string
   ///         and a templated function which can extract typed data from statement
   //--------------------------------------------------------------
   class CSQLite3Extension
   {
   public:
      //--------------------------------------------------------------
      ///\brief  Destructor
      //--------------------------------------------------------------
      virtual ~CSQLite3Extension() {}

      //--------------------------------------------------------------
      ///\brief  Extract a column value in a typed way
      ///\param [in]    pStmt    The current statement
      ///\param [in]    i        The index (zero based) of the targeted column
      ///\template      TValue   The type of the expectted value
      ///\return The value from the column.
      //--------------------------------------------------------------
      template<class TValue>
      static inline TValue extractData(sqlite3_stmt * pStatement, int nCol);
   };

   //--------------------------------------------------------------
   ///\brief  Default method. Extract data as text and use CStringExtension to convert it in good type
   //--------------------------------------------------------------
   template<class TValue>
   inline TValue CSQLite3Extension::extractData(sqlite3_stmt * pStatement, int nCol)
   {
      //as default way, read text from database and extractData it with c++ side functions
      return shared::CStringExtension::parse<TValue>((char*)sqlite3_column_text(pStatement, nCol));
   }

   //--------------------------------------------------------------
   ///\brief  Override method for type = int (use specific sqlite3 function)
   //--------------------------------------------------------------
   template<>
   inline int CSQLite3Extension::extractData(sqlite3_stmt * pStatement, int nCol)
   {
      return sqlite3_column_int(pStatement, nCol);
   }

   //--------------------------------------------------------------
   ///\brief  Override method for type = float (use specific sqlite3 function)
   //--------------------------------------------------------------
   template<>
   inline float CSQLite3Extension::extractData(sqlite3_stmt * pStatement, int nCol)
   {
      return (float)sqlite3_column_double(pStatement, nCol);
   }

   //--------------------------------------------------------------
   ///\brief  Override method for type = int (use specific sqlite3 function)
   //--------------------------------------------------------------
   template<>
   inline double CSQLite3Extension::extractData(sqlite3_stmt * pStatement, int nCol)
   {
      return sqlite3_column_double(pStatement, nCol);
   }

   //--------------------------------------------------------------
   ///\brief  Override method for type = unsigned char * (use specific sqlite3 function)
   //--------------------------------------------------------------
   template<>
   inline unsigned char* CSQLite3Extension::extractData(sqlite3_stmt * pStatement, int nCol)
   {
      return (unsigned char*)sqlite3_column_blob(pStatement, nCol);
   }

   //--------------------------------------------------------------
   ///\brief  Override method for type = std::string (use specific sqlite3 function)
   //--------------------------------------------------------------
   template<>
   inline std::string CSQLite3Extension::extractData(sqlite3_stmt * pStatement, int nCol)
   {
      return std::string((char*)sqlite3_column_text(pStatement, nCol));
   }

   //--------------------------------------------------------------
   ///\brief  Override method for type = std::string (use specific sqlite3 function)
   //--------------------------------------------------------------
   template<>
   inline boost::tuples::null_type CSQLite3Extension::extractData(sqlite3_stmt * pStatement, int nCol)
   {
      return boost::tuples::null_type();
   }

   //--------------------------------------------------------------
   ///\brief  Override method for type = bool (use specific sqlite3 function)
   //--------------------------------------------------------------
   template<>
   inline bool CSQLite3Extension::extractData(sqlite3_stmt * pStatement, int nCol)
   {
      return sqlite3_column_int(pStatement, nCol) == 1;
   }

   //--------------------------------------------------------------
   ///\brief  Override method for type = boost::posix_time::ptime (use specific sqlite3 function)
   //--------------------------------------------------------------
   template<>
   inline boost::posix_time::ptime CSQLite3Extension::extractData(sqlite3_stmt * pStatement, int nCol)
   {
      return boost::posix_time::from_iso_string(extractData<std::string>(pStatement, nCol));
   }


} //namespace adapters
} //namespace sqlite
} //namespace database 

   