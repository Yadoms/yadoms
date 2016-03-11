#include "stdafx.h"
#include "DatabaseColumn.h"


namespace database { 
namespace common {

   CDatabaseColumn::CDatabaseColumn(const std::string & name)
      :m_name(name)
   {
   }
   
   CDatabaseColumn::~CDatabaseColumn()
   {
   }
   
   const std::string & CDatabaseColumn::GetName() const
   {
      return m_name;
   }
   
} //namespace common
} //namespace database 

