#pragma once

#include "EntityHelpers.hpp"

#define DELCARE_FIELD_CCONFIGURATION(t,n)   DELCARE_FIELD(CConfiguration,t,n)

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
   DELCARE_FIELD_CCONFIGURATION(std::string, Section);
   DELCARE_FIELD_CCONFIGURATION(std::string, Name);
   DELCARE_FIELD_CCONFIGURATION(std::string, Value);
   DELCARE_FIELD_CCONFIGURATION(std::string, DefaultValue);
   DELCARE_FIELD_CCONFIGURATION(std::string, Description);
   DELCARE_FIELD_CCONFIGURATION(boost::gregorian::date, LastModificationDate);
};