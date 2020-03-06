#include "stdafx.h"
#include "ExtraQueryData.h"

namespace pluginSystem
{
   CExtraQueryData::CExtraQueryData(const std::string& query,
                                    const shared::CDataContainerSharedPtr& data,
                                    const std::string& device)
      : m_query(query),
        m_data(data),
        m_device(device)
   {
   }

   CExtraQueryData::~CExtraQueryData()
   {
   }

   const std::string& CExtraQueryData::query() const
   {
      return m_query;
   }

   const shared::CDataContainerSharedPtr& CExtraQueryData::data() const
   {
      return m_data;
   }

   const std::string& CExtraQueryData::device() const
   {
      return m_device;
   }
} // namespace pluginSystem	


