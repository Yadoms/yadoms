#pragma once

#include "database/common/QueryFunctions.h"

namespace database {  namespace sqlite {

   //-----------------------------
   ///\brief  Provide specific SQLite queries functions (could be used inside queries, like "sum", "min", "coalesce", ...
   //-----------------------------
   class CSQLiteQueryFunctions : public database::common::CQueryFunctions
   {
   public:
      //-----------------------------
      ///\brief Constructor
      //-----------------------------
      CSQLiteQueryFunctions();

      //-----------------------------
      ///\brief Destructor
      //-----------------------------
      virtual ~CSQLiteQueryFunctions();


      //--------------------------------------------------------------
      ///\brief	generate coalesce function ( ie: ifnull(field0, default) )
      ///\param [in]	field       The field or query
      ///\param [in]	valueIfNull The fallback value
      ///\return The query function
      //--------------------------------------------------------------
      virtual const std::string coalesce(const std::string & fieldOrQuery, const std::string & valueIfNull);
   };

} //namespace sqlite
} //namespace database 

