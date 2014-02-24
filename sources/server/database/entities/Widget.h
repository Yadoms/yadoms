#pragma once

#include "EntityHelpers.hpp"

#define DELCARE_FIELD_CWIDGET(t,n)   DELCARE_FIELD(CWidget,t,n)


namespace database { 
namespace entities {

   class CWidget
   {
   public:
      CWidget()
      {
         m_Id_filled = false;
         m_IdPage_filled = false;
         m_Name_filled = false;
         m_SizeX_filled = false;
         m_SizeY_filled = false;
         m_PositionX_filled = false;
         m_PositionY_filled = false;
         m_Configuration_filled = false;
      }

      virtual ~CWidget()
      {
      }

      DELCARE_FIELD_CWIDGET(int, Id);
      DELCARE_FIELD_CWIDGET(int, IdPage);
      DELCARE_FIELD_CWIDGET(std::string, Name);
      DELCARE_FIELD_CWIDGET(int, SizeX);
      DELCARE_FIELD_CWIDGET(int, SizeY);
      DELCARE_FIELD_CWIDGET(int, PositionX);
      DELCARE_FIELD_CWIDGET(int, PositionY);
      DELCARE_FIELD_CWIDGET(std::string, Configuration);
   };

} //namespace entities
} //namespace database

