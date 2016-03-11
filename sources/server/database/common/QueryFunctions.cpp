#include "stdafx.h"
#include "Query.h"
#include "QueryFunctions.h"

namespace database { 
namespace common {
   /*
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

   const std::string CQueryFunctions::minWithCast(const std::string & fieldOrQuery)
   {
      return min(castNumeric(fieldOrQuery));
   }

   const std::string CQueryFunctions::maxWithCast(const std::string & fieldOrQuery)
   {
      return max(castNumeric(fieldOrQuery));
   }

   const std::string CQueryFunctions::averageWithCast(const std::string & fieldOrQuery)
   {
      return average(castNumeric(fieldOrQuery));
   }

   const std::string CQueryFunctions::coalesce(const std::string & field, const std::string & valueIfNull)
   {
      return (boost::format("coalesce(%1%, %2%)") % field % valueIfNull).str();
   }  
   
   const std::string CQueryFunctions::coalesceNumeric(const std::string & field, const int valueIfNull)
   {
      return (boost::format("coalesce(%1%, %2%)") % field % valueIfNull).str();
   }

   const std::string CQueryFunctions::coalesce(const CQuery & subQuery, const std::string & valueIfNull)
   {
      return coalesce("(" + subQuery.str() + ")", valueIfNull);
   }

   const std::string CQueryFunctions::coalesceNumeric(const CQuery & subQuery, const int valueIfNull)
   {
      return coalesceNumeric(castNumeric("(" + subQuery.str() + ")"), valueIfNull);
   }

   const std::string CQueryFunctions::cast(const std::string & fieldOrQuery, const std::string &type)
   {
      return (boost::format("cast(%1% AS %2%)") % fieldOrQuery % type).str();
   }

   const std::string CQueryFunctions::formatDateToSql(const boost::posix_time::ptime & time)
   {
      return boost::posix_time::to_iso_string(time);
   }
   */
} //namespace common
} //namespace database 

