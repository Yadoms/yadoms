#pragma once

#include "../common/Query.h"


namespace database
{
   namespace pgsql
   {
#define CQUERY_OP_ILIKE " ILIKE "

      //
      /// \brief  Class used to create queries
      //
      class CPgsqlQuery : public common::CQuery
      {
      public:
         //
         /// \brief           Constructor
         //
         CPgsqlQuery();

         //
         /// \brief           Destructor
         //
         virtual ~CPgsqlQuery();

         //
         /// \brief           Start a query with 'SELECT EXISTS(subquery)'
         /// \return          A reference to itself to allow method chaining
         //
         CPgsqlQuery& SelectExists(common::CQuery& subQuery);

         //
         /// \brief        Start a create databse query
         /// \param [in]   name  The database name to create
         /// \return       A reference to itself to allow method chaining
         //
         CPgsqlQuery& CreateDatabase(const std::string& name);

         // Overriding common::CQuery
         std::string functionDateToIsoString(const std::string& sqlPart) override;
         // [END] - Overriding common::CQuery 
      };
   } //namespace pgsql
} //namespace database 


