#pragma once

#include "EntityHelpers.hpp"

#define DELCARE_FIELD_CEVENT_LOGGER(t,n)   DELCARE_FIELD(CEventLogger,t,n)

class CEventLogger
{
public:
   CEventLogger()
   {
   }

   virtual ~CEventLogger()
   {
   }

   DELCARE_FIELD_CEVENT_LOGGER(int, Id);
   DELCARE_FIELD_CEVENT_LOGGER(boost::posix_time::ptime, EventDate);
   DELCARE_FIELD_CEVENT_LOGGER(int , EventType);
   DELCARE_FIELD_CEVENT_LOGGER(std::string, OptionalData);
};

