#pragma once

#include "sqlite3.h"
#include <shared/StringExtension.h>
#include <shared/plugin/Information/IInformation.h>
#include "database/entities/HardwareEventLogger.h"
#include "database/entities/Configuration.h"

//--------------------------------------------------------------
///\brief   Class (static) used to extend sqlite3 functions.
///         It provides a method to handle utf8 in std::string
///         and a templated function which can extract typed data from statement
//--------------------------------------------------------------
class CSQLite3Extension
{
public:
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
///\brief  Dafault method. Extract data as text and use CStringExtension to convert it in good type
//--------------------------------------------------------------
template<class TValue>
inline TValue CSQLite3Extension::extractData(sqlite3_stmt * pStatement, int nCol)
{
   //as default way, read text from database and extractData it with c++ side functions
   return CStringExtension::parse<TValue>((char*)sqlite3_column_text(pStatement, nCol));
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
   return boost::posix_time::time_from_string(extractData<std::string>(pStatement, nCol));
}




   