#include "stdafx.h"

#include "SQLiteResultHandler.h"
#include <Poco/DateTimeParser.h>


namespace database {
   namespace sqlite {



      CSQLiteResultHandler::CSQLiteResultHandler(sqlite3_stmt *pStatement)
         :m_pStatement(pStatement)
      {

      }

      CSQLiteResultHandler::~CSQLiteResultHandler()
      {

      }

      // database::common::IResultHandler implementation
      int CSQLiteResultHandler::getColumnCount()
      {
         return sqlite3_column_count(m_pStatement);
      }

      std::string CSQLiteResultHandler::getColumnName(const int columnIndex)
      {
         return std::string(sqlite3_column_name(m_pStatement, columnIndex));
      }

      bool CSQLiteResultHandler::next_step()
      {
         return sqlite3_step(m_pStatement) == SQLITE_ROW;
      }

      std::string CSQLiteResultHandler::extractValueAsString(const int columnIndex)
      {
         return std::string((char*)sqlite3_column_text(m_pStatement, columnIndex));
      }

      int CSQLiteResultHandler::extractValueAsInt(const int columnIndex)
      {
         return sqlite3_column_int(m_pStatement, columnIndex);
      }

      float CSQLiteResultHandler::extractValueAsFloat(const int columnIndex)
      {
         return (float)sqlite3_column_double(m_pStatement, columnIndex);
      }

      double CSQLiteResultHandler::extractValueAsDouble(const int columnIndex)
      {
         return sqlite3_column_double(m_pStatement, columnIndex);
      }

      unsigned char* CSQLiteResultHandler::extractValueAsBlob(const int columnIndex)
      {
         return (unsigned char*)sqlite3_column_blob(m_pStatement, columnIndex);
      }

      bool CSQLiteResultHandler::extractValueAsBool(const int columnIndex)
      {
         return sqlite3_column_int(m_pStatement, columnIndex) == 1;
      }

      bool CSQLiteResultHandler::isValueNull(const int columnIndex)
      {
         return sqlite3_column_type(m_pStatement, columnIndex) == SQLITE_NULL;
      }

      boost::posix_time::ptime CSQLiteResultHandler::extractValueAsBoostTime(const int columnIndex)
      {
         return boost::posix_time::from_iso_string(extractValueAsString(columnIndex));
      }

      Poco::DateTime CSQLiteResultHandler::extractValueAsPocoTime(const int columnIndex)
      {
         int timeZoneDifferential;
         return Poco::DateTimeParser::parse("%Y%m%dT%H%M%S", extractValueAsString(columnIndex), timeZoneDifferential);
      }

      shared::CDataContainerSharedPtr CSQLiteResultHandler::extractValueAsDataContainer(const int columnIndex)
      {
         return new_CDataContainerSharedPtrP(extractValueAsString(columnIndex));
      }


   } //namespace sqlite
} //namespace database 

