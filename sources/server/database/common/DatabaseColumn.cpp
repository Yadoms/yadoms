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
      
   
   CDatabaseTable::CDatabaseTable(const std::string & name)
      :m_name(name)
   {
   }
   
   CDatabaseTable::~CDatabaseTable()
   {
   }
   
   const std::string & CDatabaseTable::GetName() const
   {
      return m_name;
   }
   
} //namespace common
} //namespace database 

