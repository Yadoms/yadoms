#include "stdafx.h"
#include "PgsqlQueryFunctions.h"

#include <shared/Log.h>

namespace database { 
namespace pgsql {

   CPgsqlQueryFunctions::CPgsqlQueryFunctions()
   { 
   }

   CPgsqlQueryFunctions::~CPgsqlQueryFunctions()
   {
   }


   const std::string CPgsqlQueryFunctions::dateToIsoString(const std::string &fieldOrQuery)
   {
      return "to_char(" + fieldOrQuery + ", 'YYYY\"-\"MM\"-\"DD\"T\"HH24\":\"MI\":\"SS')";
   }

   const std::string CPgsqlQueryFunctions::castNumeric(const std::string & fieldOrQuery)
   {
      return cast(fieldOrQuery, "numeric");
   }


} //namespace pgsql
} //namespace database 

