#pragma once

#include "EntityHelpers.hpp"

class CConfiguration
{
public:
   CConfiguration()
   {
   }

   virtual ~CConfiguration()
   {
   }

public:
   DELCARE_FIELD(std::string, Section);
   DELCARE_FIELD(std::string, Name);
   DELCARE_FIELD(std::string, Value);
   DELCARE_FIELD(std::string, DefaultValue);
   DELCARE_FIELD(std::string, Description);
   DELCARE_FIELD(boost::gregorian::date, LastModificationDate);
};