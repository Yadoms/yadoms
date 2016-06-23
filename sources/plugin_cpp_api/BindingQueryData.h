#pragma once

#include <shared/plugin/yPluginApi/IBindingQueryData.h>

namespace plugin_cpp_api
{
   class CBindingQueryData : public shared::plugin::yPluginApi::IBindingQueryData
   {
   public:
      explicit CBindingQueryData(const std::string& query);
      virtual ~CBindingQueryData();

      // IBindingQueryData Implementation
      const std::string& getQuery() const override;
      // [END] IBindingQueryData Implementation

   private:
      const std::string m_query;
   };
} // namespace plugin_cpp_api	


