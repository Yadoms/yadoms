#pragma once

#include "EntityHelpers.hpp"

#define DELCARE_FIELD_CPAGE(t,n)   DELCARE_FIELD(CPage,t,n)

class CPage
{
public:
   CPage()
   {
   }

   virtual ~CPage()
   {
   }

   DELCARE_FIELD_CPAGE(int, Id);
   DELCARE_FIELD_CPAGE(std::string, Name);
   DELCARE_FIELD_CPAGE(int, PageOrder);
};
