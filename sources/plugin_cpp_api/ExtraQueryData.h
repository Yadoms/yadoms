#pragma once

#include <shared/plugin/yPluginApi/IExtraQueryData.h>

namespace plugin_cpp_api
{
   class CExtraQueryData : public shared::plugin::yPluginApi::IExtraQueryData
   {
   public:
      explicit CExtraQueryData(const std::string& query,
                               const shared::CDataContainer& data);
      virtual ~CExtraQueryData();

      // IExtraQueryData Implementation
      const std::string& query() const override;
      const shared::CDataContainer& data() const override;
      // [END] IExtraQueryData Implementation

   private:
      const std::string m_query;
      const shared::CDataContainer m_data;
   };
} // namespace plugin_cpp_api	


