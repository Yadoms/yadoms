#pragma once

#include <shared/plugin/yPluginApi/IExtraQueryData.h>

namespace plugin_cpp_api
{
   class CExtraQueryData : public shared::plugin::yPluginApi::IExtraQueryData
   {
   public:
      explicit CExtraQueryData(const std::string& query,
                               const shared::CDataContainerSharedPtr& data,
                               const std::string &device);
      virtual ~CExtraQueryData();

      // IExtraQueryData Implementation
      const std::string& query() const override;
      const shared::CDataContainerSharedPtr& data() const override;
      const std::string& device() const override;
      // [END] IExtraQueryData Implementation

   private:
      const std::string m_query;
      const shared::CDataContainerSharedPtr m_data;
      const std::string m_device;
   };
} // namespace plugin_cpp_api	


