#include "stdafx.h"
#include "PgsqlQuery.h"

namespace database { namespace pgsql { 

   CPgsqlQuery::CPgsqlQuery()
   {
   }

   CPgsqlQuery::~CPgsqlQuery()
   {
   }

   CPgsqlQuery & CPgsqlQuery::SelectExists(common::CQuery & subQuery)
   {
      ChangeQueryType(kSelect);
      std::ostringstream ss;
      ss << "SELECT EXISTS( " << subQuery.str() << ") ";
      Append(ss); 
      return *this;
   }

   CPgsqlQuery & CPgsqlQuery::CreateDatabase(const std::string & name)
   {
      ChangeQueryType(kCreate);
      std::ostringstream ss;
      ss << "CREATE DATABASE " << name;
      Append(ss);
      return *this;
   }


} //namespace pgsql
} //namespace database 

