#include "stdafx.h"
#include "QueryFunctions.h"

namespace database { 
namespace common {

   CQueryFunctions::CQueryFunctions() 
   { 
   }

   CQueryFunctions::~CQueryFunctions() 
   {
   }

   const std::string CQueryFunctions::min(const std::string & fieldOrQuery)
   {
      return (boost::format("min(%1%)") % fieldOrQuery).str();
   }

   const std::string CQueryFunctions::max(const std::string & fieldOrQuery)
   {
      return (boost::format("max(%1%)") % fieldOrQuery).str();
   }

   const std::string CQueryFunctions::average(const std::string & fieldOrQuery)
   {
      return (boost::format("avg(%1%)") % fieldOrQuery).str();
   }

   const std::string CQueryFunctions::coalesce(const std::string & fieldOrQuery, const std::string & valueIfNull)
   {
      return (boost::format("coalesce(%1%, %2%)") % fieldOrQuery % valueIfNull).str();
   }

      


} //namespace common
} //namespace database 

