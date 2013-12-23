#pragma once

#include "EntityHelpers.hpp"

#define DELCARE_FIELD_CDEVICE(t,n)   DELCARE_FIELD(CDevice,t,n)

class CDevice
{
public:
   CDevice()
   {
   }

   virtual ~CDevice()
   {
   }

   DELCARE_FIELD_CDEVICE(int, Id);
   DELCARE_FIELD_CDEVICE(std::string, DataSource);
   DELCARE_FIELD_CDEVICE(std::string, Name);
   DELCARE_FIELD_CDEVICE(std::string, Configuration);
};
