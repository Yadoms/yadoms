#pragma once

#include "EntityHelpers.hpp"

#define DELCARE_FIELD_CWIDGET(t,n)   DELCARE_FIELD(CWidget,t,n)

namespace server { 
namespace database { 
namespace entities {

   class CWidget
   {
   public:
      CWidget()
      {
         m_Id = 0;
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
} //namespace server
