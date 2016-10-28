#include "stdafx.h"
#include "ExtraQuery.h"

namespace pluginSystem
{

   CExtraQuery::CExtraQuery(const std::string& command, const shared::CDataContainer & data)
      :m_command(command), m_data(data)
   {
   }

   CExtraQuery::~CExtraQuery()
   {
   }

   const std::string& CExtraQuery::getQuery() const
   {
      return m_command;
   }

   const shared::CDataContainer & CExtraQuery::getData() const
   {
      return m_data;
   }



} // namespace pluginSystem	
