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
   } //namespace sqlite
} //namespace database 


