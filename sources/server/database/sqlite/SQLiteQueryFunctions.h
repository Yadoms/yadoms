#pragma once

#include "database/common/QueryFunctions.h"

namespace database {  namespace sqlite {
   /*
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


      // database::common::CQueryFunctions implemententation/overloading
      virtual const std::string coalesce(const std::string & fieldOrQuery, const std::string & valueIfNull);
      virtual const std::string dateToIsoString(const std::string &columnName);
      virtual const std::string castNumeric(const std::string & fieldOrQuery);
      // [END] - database::common::CQueryFunctions implemententation/overloading

   };
   */
} //namespace sqlite
} //namespace database 

