#pragma once

#include "../common/Query.h"


namespace database { namespace pgsql { 

#define CQUERY_OP_ILIKE " ILIKE "

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

      //
      /// \brief        Start a create databse query
      /// \param [in]   name  The database name to create
      /// \return       A reference to itself to allow method chaining
      //
      common::CQuery & CreateDatabase(const std::string & name);
   };


} //namespace common
} //namespace database 

