#include "stdafx.h"
#include "CustomQueryData.h"

namespace pluginSystem
{

   CCustomQueryData::CCustomQueryData(const std::string& query)
      :m_query(query)
   {
   }
      
   CCustomQueryData::~CCustomQueryData() 
   {
   }

   const std::string& CCustomQueryData::getQuery() const
   {
      return m_query;
   }

  

} // namespace pluginSystem	
