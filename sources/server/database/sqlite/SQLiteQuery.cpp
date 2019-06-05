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
         return (boost::format("(strftime('%s', isodate(%1%)) * 1000)") % sqlPart).str();
      }

      std::string CSQLiteQuery::functionSubstring(const std::string& sqlPart, int offset, int count)
      {
         return (boost::format("substr(%1%, %2%, %3%)") % sqlPart % (offset + 1)  % count).str();
      }
   } //namespace sqlite
} //namespace database 


