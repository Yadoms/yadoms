#pragma once

#include "EntityHelpers.hpp"

#define DELCARE_FIELD_CPAGE(t,n)   DELCARE_FIELD(CPage,t,n)


namespace database { 
namespace entities {

   class CPage
   {
   public:
      CPage()
      {
         m_Id_filled = false;
         m_Name_filled = false;
         m_PageOrder_filled = false;
      }

      virtual ~CPage()
      {
      }

      DELCARE_FIELD_CPAGE(int, Id);
      DELCARE_FIELD_CPAGE(std::string, Name);
      DELCARE_FIELD_CPAGE(int, PageOrder);
   };

} //namespace entities
} //namespace database

