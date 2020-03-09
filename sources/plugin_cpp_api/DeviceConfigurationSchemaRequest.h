#pragma once
#include <plugin_IPC/yadomsToPlugin.pb.h>
#include <shared/plugin/yPluginApi/IDeviceConfigurationSchemaRequest.h>

namespace plugin_cpp_api
{
   class CDeviceConfigurationSchemaRequest : public shared::plugin::yPluginApi::IDeviceConfigurationSchemaRequest
   {
   public:
      CDeviceConfigurationSchemaRequest(const plugin_IPC::toPlugin::DeviceConfigurationSchemaRequest& msg,
                                        boost::function1<void, const shared::CDataContainerSharedPtr&> sucessCallback,
                                        boost::function1<void, const std::string&> errorCallback);
      virtual ~CDeviceConfigurationSchemaRequest();

      // IDeviceConfigurationSchemaRequest Implementation
      std::string device() const override;
      void sendSuccess(const shared::CDataContainerSharedPtr& deviceConfigurationSchema) override;
      void sendError(const std::string& errorMessage) override;
      // [END] IDeviceConfigurationSchemaRequest Implementation

   private:
      const std::string m_device;
      boost::function1<void, const shared::CDataContainerSharedPtr&> m_sucessCallback;
      boost::function1<void, const std::string&> m_errorCallback;
   };
} // namespace plugin_cpp_api	


