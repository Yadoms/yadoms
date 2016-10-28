#include "stdafx.h"
#include "ExtraQueryData.h"

namespace pluginSystem
{
   CExtraQueryData::CExtraQueryData(const std::string& query,
                                    const shared::CDataContainer& data)
      : m_query(query),
        m_data(data)
   {
   }

   CExtraQueryData::~CExtraQueryData()
   {
   }

   const std::string& CExtraQueryData::query() const
   {
      return m_query;
   }

   const shared::CDataContainer& CExtraQueryData::data() const
   {
      return m_data;
   }
} // namespace pluginSystem	


