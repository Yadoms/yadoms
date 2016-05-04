#include "stdafx.h"
#include "BindingQueryData.h"


namespace plugin_cpp_api
{
   CBindingQueryData::CBindingQueryData(const std::string& query)
      : m_query(query)
   {
   }

   CBindingQueryData::~CBindingQueryData()
   {
   }

   const std::string& CBindingQueryData::getQuery() const
   {
      return m_query;
   }
} // namespace plugin_cpp_api	


