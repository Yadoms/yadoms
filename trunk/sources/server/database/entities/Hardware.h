#pragma once

#include "EntityHelpers.hpp"

#define DELCARE_FIELD_CHARDWARE(t,n)   DELCARE_FIELD(CHardware,t,n)

class CHardware
{
public:
   CHardware()
   {
   }

   virtual ~CHardware()
   {
   }

   DELCARE_FIELD_CHARDWARE(int, Id);
   DELCARE_FIELD_CHARDWARE(std::string, Name);
   DELCARE_FIELD_CHARDWARE(std::string, PluginName);
   DELCARE_FIELD_CHARDWARE(std::string, Configuration);
   DELCARE_FIELD_CHARDWARE(bool, Enabled);
   DELCARE_FIELD_CHARDWARE(bool, Deleted);
};