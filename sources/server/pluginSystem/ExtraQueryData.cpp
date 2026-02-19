#include "stdafx.h"
#include "ExtraQueryData.h"

#include <utility>

namespace pluginSystem
{
   CExtraQueryData::CExtraQueryData(std::string query,
                                    boost::shared_ptr<shared::CDataContainer> data,
                                    std::string deviceName)
      : m_query(std::move(query)),
        m_data(std::move(data)),
        m_deviceName(std::move(deviceName))
   {
   }

   const std::string& CExtraQueryData::query() const
   {
      return m_query;
   }

   const boost::shared_ptr<shared::CDataContainer>& CExtraQueryData::data() const
   {
      return m_data;
   }

   const std::string& CExtraQueryData::device() const
   {
      return m_deviceName;
   }
} // namespace pluginSystem	
