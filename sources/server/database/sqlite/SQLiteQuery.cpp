#include "stdafx.h"
#include "SQLiteQuery.h"

namespace database
{
   namespace sqlite
   {
      CSQLiteQuery::CSQLiteQuery()
      {
      }

      CSQLiteQuery::~CSQLiteQuery()
      {
      }

      std::string CSQLiteQuery::functionDateToIsoString(const std::string& sqlPart)
      {
         return std::string("(strftime('%s', isodate(") + sqlPart + ")) * 1000)";
      }

      std::string CSQLiteQuery::functionSubstring(const std::string& sqlPart, int offset, int count)
      {
         return std::string("substr(") + sqlPart + ", " + std::to_string(offset + 1) + ", " + std::to_string(count) + ")";
      }
   } //namespace sqlite
} //namespace database 


