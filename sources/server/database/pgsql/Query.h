#pragma once

#include "../common/Query.h"


namespace database { namespace pgsql { 

   //
   /// \brief  Class used to create queries
   //
   class CQuery : public common::CQuery
   {
   public:
      //
      /// \brief           Constructor
      //
      CQuery();

      //
      /// \brief           Destructor
      //
      virtual ~CQuery();

      //
      /// \brief           Start a query with 'SELECT EXISTS(subquery)'
      /// \return          A reference to itself to allow method chaining
      //
      common::CQuery & SelectExists(common::CQuery & subQuery);
   };


} //namespace common
} //namespace database 

