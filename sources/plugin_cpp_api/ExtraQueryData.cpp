#include "stdafx.h"
#include "ExtraQueryData.h"


namespace plugin_cpp_api
{
   CExtraQueryData::CExtraQueryData(const std::string& query, const shared::CDataContainer& data, const std::string& device)
      : m_query(query), m_data(data), m_device(device)
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

   const std::string& CExtraQueryData::device() const
   {
      return m_device;
   }
} // namespace plugin_cpp_api	


