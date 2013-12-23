#pragma once

#include "EntityHelpers.hpp"

#define DELCARE_FIELD_CKEYWORDS(t,n)   DELCARE_FIELD(CKeywords,t,n)

class CKeywords
{
public:
   CKeywords()
   {
   }

   virtual ~CKeywords()
   {
   }

   DELCARE_FIELD_CKEYWORDS(std::string, Name);
};
