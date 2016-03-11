#pragma once

#include "Query.h"

namespace database { 
namespace common { 
   /*
   //-----------------------------
   ///\brief  Provide queries functions (could be used inside queries, like "sum", "min", "coalesce", ...
   //-----------------------------
   class CQueryFunctions
   {
   protected:
      //-----------------------------
      ///\brief Constructor
      //-----------------------------
      CQueryFunctions();

   public:
      //-----------------------------
      ///\brief Destructor
      //-----------------------------
      virtual ~CQueryFunctions();
      
      //--------------------------------------------------------------
      ///\brief	generate min function ( ie: min(field0) )
      ///\param [in]	field    The field or query
      ///\return The query function
      //--------------------------------------------------------------
      virtual const std::string min(const std::string & fieldOrQuery);

      //--------------------------------------------------------------
      ///\brief	generate min function ( ie: min(field0) ) with numeric cast
      ///\param [in]	field    The field or query
      ///\return The query function
      //--------------------------------------------------------------
      virtual const std::string minWithCast(const std::string & fieldOrQuery);

      //--------------------------------------------------------------
      ///\brief	generate max function ( ie: max(field0) )
      ///\param [in]	field    The field or query
      ///\return The query function
      //--------------------------------------------------------------
      virtual const std::string max(const std::string & fieldOrQuery);

      //--------------------------------------------------------------
      ///\brief	generate max function ( ie: max(field0) ) with numeric cast
      ///\param [in]	field    The field or query
      ///\return The query function
      //--------------------------------------------------------------
      virtual const std::string maxWithCast(const std::string & fieldOrQuery);

      //--------------------------------------------------------------
      ///\brief	generate average function ( ie: average(field0) )
      ///\param [in]	field    The field or query
      ///\return The query function
      //--------------------------------------------------------------
      virtual const std::string average(const std::string & fieldOrQuery);

      //--------------------------------------------------------------
      ///\brief	generate average function ( ie: average(field0) ) with numeric cast
      ///\param [in]	field    The field or query
      ///\return The query function
      //--------------------------------------------------------------
      virtual const std::string averageWithCast(const std::string & fieldOrQuery);

      //--------------------------------------------------------------
      ///\brief	generate coalesce function ( ie: coalesce(field0, default) )
      ///\param [in]	field       The field or query
      ///\param [in]	valueIfNull The fallback value
      ///\return The query function
      //--------------------------------------------------------------
      virtual const std::string coalesce(const std::string & field, const std::string & valueIfNull);    
      
      //--------------------------------------------------------------
      ///\brief	generate coalesce function ( ie: coalesce(field0, default) )
      ///\param [in]	field       The field or query
      ///\param [in]	valueIfNull The fallback value
      ///\return The query function
      //--------------------------------------------------------------
      virtual const std::string coalesceNumeric(const std::string & field, const int valueIfNull);

      //--------------------------------------------------------------
      ///\brief	generate coalesce function ( ie: coalesce(field0, default) )
      ///\param [in]	field       The field or query
      ///\param [in]	valueIfNull The fallback value
      ///\return The query function
      //--------------------------------------------------------------
      virtual const std::string coalesce(const CQuery & subQuery, const std::string & valueIfNull);
      
      //--------------------------------------------------------------
      ///\brief	generate coalesce function ( ie: coalesce(field0, default) )
      ///\param [in]	field       The field or query
      ///\param [in]	valueIfNull The fallback value
      ///\return The query function
      //--------------------------------------------------------------
      virtual const std::string coalesceNumeric(const CQuery & subQuery, const int defaultValue);

      //--------------------------------------------------------------
      ///\brief	generate cast function ( ie: CAST (field0 AS numeric) )
      ///\param [in]	field       The field or query
      ///\param [in]	type        The typing cast
      ///\return The query function
      //--------------------------------------------------------------
      virtual const std::string cast(const std::string & fieldOrQuery, const std::string &type);


      //--------------------------------------------------------------
      ///\brief	generate a date to iso string function
      ///\param [in]	fieldOrQuery       The field or query (providing dates) to format as IsoString
      ///\return The query function
      //--------------------------------------------------------------
      virtual const std::string dateToIsoString(const std::string &fieldOrQuery) = 0;

      //--------------------------------------------------------------
      ///\brief	generate cast function ( ie: CAST (field0 AS numeric) )
      ///\param [in]	field       The field or query
      ///\return The query function
      //--------------------------------------------------------------
      virtual const std::string castNumeric(const std::string & fieldOrQuery) = 0;


      virtual const std::string formatDateToSql(const boost::posix_time::ptime & time);
      
   };
*/
} //namespace common
} //namespace database 

