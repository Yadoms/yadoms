#include "stdafx.h"
#include "SQLiteQueryFunctions.h"

namespace database { 
namespace sqlite {

   CSQLiteQueryFunctions::CSQLiteQueryFunctions()
   { 
   }

   CSQLiteQueryFunctions::~CSQLiteQueryFunctions()
   {
   }

   const std::string CSQLiteQueryFunctions::coalesce(const std::string & fieldOrQuery, const std::string & valueIfNull)
   {
      return (boost::format("ifnull(%1%, %2%)") % fieldOrQuery % valueIfNull).str();
   }

      


} //namespace sqlite
} //namespace database 

