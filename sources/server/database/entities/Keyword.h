#pragma once

#include "EntityHelpers.hpp"

#define DELCARE_FIELD_CKEYWORD(t,n)   DELCARE_FIELD(CKeyword,t,n)

class CKeyword
{
public:
   CKeyword()
   {
   }

   virtual ~CKeyword()
   {
   }

   DELCARE_FIELD_CKEYWORD(std::string, Name);
};
