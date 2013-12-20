#pragma once

#include "EntityHelpers.hpp"

#define DELCARE_FIELD_CACQUISITIONS(t,n)   DELCARE_FIELD(CAcquisitions,t,n)

class CAcquisitions
{
public:
   CAcquisitions()
   {
   }

   virtual ~CAcquisitions()
   {
   }

   DELCARE_FIELD_CACQUISITIONS(int, Id);
   DELCARE_FIELD_CACQUISITIONS(std::string, Source);
   DELCARE_FIELD_CACQUISITIONS(std::string, Keyword);
   DELCARE_FIELD_CACQUISITIONS(std::string, Value);
   DELCARE_FIELD_CACQUISITIONS(boost::gregorian::date, Date);
};