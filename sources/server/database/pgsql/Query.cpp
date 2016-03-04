#include "stdafx.h"
#include "Query.h"

namespace database { namespace pgsql { 

   CQuery::CQuery()
   {
   }

   CQuery::~CQuery()
   {
   }

   common::CQuery & CQuery::SelectExists(common::CQuery & subQuery)
   {
      ChangeQueryType(kSelect);
      std::ostringstream ss;
      ss << "SELECT EXISTS( " << subQuery.str() << ") ";
      return Append(ss); 
   }

   common::CQuery & CQuery::CreateDatabase(const std::string & name)
   {
      ChangeQueryType(kCreate);
      std::ostringstream ss;
      ss << "CREATE DATABASE " << name;
      return Append(ss);
   }


} //namespace pgsql
} //namespace database 

