#include "stdafx.h"
#include "PgsqlQuery.h"

namespace database
{
   namespace pgsql
   {
      CPgsqlQuery::CPgsqlQuery()
         : common::CQuery(true)
      {
      }

      CPgsqlQuery::~CPgsqlQuery()
      {
      }

      CPgsqlQuery& CPgsqlQuery::SelectExists(const common::CQuery& subQuery)
      {
         ChangeQueryType(kSelect);
         std::ostringstream ss;
         ss << "SELECT EXISTS( " << subQuery.str() << ") ";
         Append(ss);
         return *this;
      }

      CPgsqlQuery& CPgsqlQuery::CreateDatabase(const std::string& name)
      {
         ChangeQueryType(kCreate);
         std::ostringstream ss;
         ss << "CREATE DATABASE " << name;
         Append(ss);
         return *this;
      }

      std::string CPgsqlQuery::functionDateToIsoString(const std::string& sqlPart)
      {
         return (boost::format("to_char(%1%, 'YYYY\"-\"MM\"-\"DD\"T\"HH24\":\"MI\":\"SS')") % sqlPart).str();
      }


      std::string CPgsqlQuery::functionSubstring(const std::string& sqlPart, int offset, int count)
      {
         return (boost::format("substring(%1% from %2% for %3%)") % sqlPart % offset % count).str();
      }
   } //namespace pgsql
} //namespace database 


