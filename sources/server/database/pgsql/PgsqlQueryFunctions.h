#pragma once

#include "database/common/QueryFunctions.h"

namespace database { 
namespace pgsql { 

   //-----------------------------
   ///\brief  Provide specific queries functions (could be used inside queries, like "sum", "min", "coalesce", ...
   //-----------------------------
   class CPgsqlQueryFunctions : public database::common::CQueryFunctions
   {
   public:
      //-----------------------------
      ///\brief Constructor
      //-----------------------------
      CPgsqlQueryFunctions();

      //-----------------------------
      ///\brief Destructor
      //-----------------------------
      virtual ~CPgsqlQueryFunctions();
   };

} //namespace pgsql
} //namespace database 

