#include "stdafx.h"
#include "ExtraQuery.h"

namespace plugin_cpp_api
{
   CExtraQuery::CExtraQuery(const toPlugin::ExtraQuery& msg)
      : m_query(msg.query()),
        m_data(msg.data())
   {
   }

   CExtraQuery::~CExtraQuery()
   {
   }

   const std::string& CExtraQuery::getQuery() const
   {
      return m_query;
   }

   const shared::CDataContainer& CExtraQuery::getData() const
   {
      return m_data;
   }
} // namespace plugin_cpp_api	


