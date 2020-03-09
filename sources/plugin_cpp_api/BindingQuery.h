#pragma once
#include <plugin_IPC/yadomsToPlugin.pb.h>
#include <shared/plugin/yPluginApi/IBindingQueryRequest.h>
#include "BindingQueryData.h"

namespace plugin_cpp_api
{
   class CBindingQuery : public shared::plugin::yPluginApi::IBindingQueryRequest
   {
   public:
      CBindingQuery(const plugin_IPC::toPlugin::BindingQuery& msg,
                    boost::function1<void, const shared::CDataContainerSharedPtr&> sucessCallback,
                    boost::function1<void, const std::string&> errorCallback);
      virtual ~CBindingQuery();

      // IBindingQueryRequest Implementation
      const shared::plugin::yPluginApi::IBindingQueryData& getData() override;
      void sendSuccess(const shared::CDataContainerSharedPtr& result) override;
      void sendError(const std::string& errorMessage) override;
      // [END] IBindingQueryRequest Implementation

   private:
      CBindingQueryData m_query;
      boost::function1<void, const shared::CDataContainerSharedPtr&> m_sucessCallback;
      boost::function1<void, const std::string&> m_errorCallback;
   };
} // namespace plugin_cpp_api	
