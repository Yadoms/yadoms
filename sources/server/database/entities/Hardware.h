#pragma once

#include "EntityHelpers.hpp"

class CHardware
{
public:
   CHardware()
   {
   }

   virtual ~CHardware()
   {
   }

public:
   DELCARE_FIELD(int, Id);
   DELCARE_FIELD(std::string, Name);
   DELCARE_FIELD(std::string, PluginName);
   DELCARE_FIELD(std::string, Configuration);
   DELCARE_FIELD(bool, Enabled);
   DELCARE_FIELD(bool, Deleted);
};